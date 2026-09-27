#include "BluetoothController.h"

BluetoothController::BluetoothController():
    serverCallbacks(*this),
    controlCallbacks(*this)
{
}

// ---
// setup
// ---

void BluetoothController::setup()
{
    inputQueue = xQueueCreate(1, sizeof(ControllerInput));

    if (inputQueue == nullptr) 
    {
        Serial.println("[BLE] Failed to create input queue.");
        return;
    }

    // initialize ble
    NimBLEDevice::init(DEVICE_NAME);

    // create ble server
    server = NimBLEDevice::createServer();

    server->setCallbacks(
        &serverCallbacks,
        false
    );

    server->advertiseOnDisconnect(true);

    // create c7 ble service
    NimBLEService* service = server->createService(SERVICE_UUID);

    // laptop -> c7 control characteristics
    controlCharacteristic = service->createCharacteristic(
        CONTROL_UUID,

        NIMBLE_PROPERTY::WRITE |
        NIMBLE_PROPERTY::WRITE_NR,

        CONTROL_PACKET_SIZE
    );

    controlCharacteristic->setCallbacks(
        &controlCallbacks
    );

    // start service
    service->start();

    // begin advertising
    NimBLEAdvertising* advertising =
        NimBLEDevice::getAdvertising();

    advertising->addServiceUUID(
        SERVICE_UUID
    );

    advertising->setName(
        DEVICE_NAME
    );

    advertising->start();

    Serial.println("[BLE] C7 Bluetooth started.");
    Serial.println("[BLE] Advertising as C7-RC.");
}

// ---
// loop
// ---

void BluetoothController::loop()
{
    if (inputQueue == nullptr)
    {
        return;
    }

    ControllerInput input;

    if (
        xQueueReceive(
            inputQueue,
            &input,
            0
        ) == pdTRUE
    )
    {
        currentInput = input;
        lastPacketMs = input.receivedAtMs;

        Serial.printf(
            "[BLE] Seq: %u | Throttle: %.2f | Steering: %.2f | Gear: %d | Mode: %d | Brake: %d | Lights: %d\n",

            currentInput.sequence,
            currentInput.throttle,
            currentInput.steering,

            static_cast<int>(
                currentInput.gear
            ),

            static_cast<int>(
                currentInput.driveMode
            ),

            currentInput.brake,
            currentInput.lights
        );
    }
}

// ---
// stop
// ----

void BluetoothController::stop()
{
    if (inputQueue != nullptr)
    {
        xQueueReset(
            inputQueue
        );
    }


    currentInput =
        ControllerInput{};

    lastPacketMs = 0;
}

//---
//getters
// ----

bool BluetoothController::isConnected() const
{
    return connected.load();
}


bool BluetoothController::hasTimedOut(
    uint32_t timeoutMs
) const
{
    if (!connected.load())
    {
        return true;
    }


    if (lastPacketMs == 0)
    {
        return true;
    }


    return (
        millis() - lastPacketMs
    ) > timeoutMs;
}


const ControllerInput&
BluetoothController::getInput() const
{
    return currentInput;
}

// ---
// server callback constructor
// ---
BluetoothController::ServerCallbacks::ServerCallbacks(
    BluetoothController& parent
)
    : parent(parent)
{
}

// ----
// connect
// ----

void BluetoothController::ServerCallbacks::onConnect(
    NimBLEServer* server,
    NimBLEConnInfo& connInfo
)
{
    parent.connected.store(true);

    Serial.println(
        "[BLE] Controller connected."
    );
}

// ----
// disconnect
// ---

void BluetoothController::ServerCallbacks::onDisconnect(
    NimBLEServer* server,
    NimBLEConnInfo& connInfo,
    int reason
)
{
    parent.connected.store(false);

    Serial.printf(
        "[BLE] Controller disconnected. Reason: %d\n",
        reason
    );
}

//---
// control callback constructor
// ----

BluetoothController::ControlCallbacks::ControlCallbacks(
    BluetoothController& parent
)
    : parent(parent)
{
}

// ----
// receive control packet
// ----

void BluetoothController::ControlCallbacks::onWrite(
    NimBLECharacteristic* characteristic,
    NimBLEConnInfo& connInfo
)
{
    std::string value =
        characteristic->getValue();


    parent.handleControlPacket(
        reinterpret_cast<const uint8_t*>(
            value.data()
        ),
        value.size()
    );
}

// ----
// decode control packet
// ----

void BluetoothController::handleControlPacket(
    const uint8_t* data,
    size_t length
)
{
    // Packet must be exactly 10 bytes
    if (length != CONTROL_PACKET_SIZE)
    {
        return;
    }


    // C7 magic byte
    if (data[0] != 0xC7)
    {
        return;
    }


    // Protocol version
    if (data[1] != 1)
    {
        return;
    }


    uint8_t sequence =
        data[2];

    uint8_t flags =
        data[3];

    uint8_t gearRaw =
        data[4];

    uint8_t driveModeRaw =
        data[5];


    // Neutral / First / Second
    if (gearRaw > 2)
    {
        return;
    }


    // Adjust this later depending
    // on your DriveMode enum
    if (driveModeRaw > 2)
    {
        return;
    }


    int16_t throttleRaw = decodeInt16LE(
        data[6],
        data[7]
    );


    int16_t steeringRaw = decodeInt16LE(
        data[8],
        data[9]
    );


    // Valid control range
    if (
        throttleRaw < -1000 ||
        throttleRaw > 1000
    )
    {
        return;
    }


    if (
        steeringRaw < -1000 ||
        steeringRaw > 1000
    )
    {
        return;
    }


    ControllerInput input;

    input.sequence = sequence;

    input.throttle =
        static_cast<float>(
            throttleRaw
        ) / 1000.0f;


    input.steering =
        static_cast<float>(
            steeringRaw
        ) / 1000.0f;


    input.gear =
        static_cast<Gear>(
            gearRaw
        );


    input.driveMode =
        static_cast<DriveMode>(
            driveModeRaw
        );


    // Bit 0 = lights
    input.lights =
        (flags & 0b00000001) != 0;


    // Bit 1 = brake
    input.brake =
        (flags & 0b00000010) != 0;


    input.receivedAtMs =
        millis();


    // Always overwrite with newest controller state
    xQueueOverwrite(
        inputQueue,
        &input
    );
}

// --
// little endian in16 decoder
// ----

int16_t BluetoothController::decodeInt16LE(
    uint8_t low,
    uint8_t high
)
{
    uint16_t value =
        static_cast<uint16_t>(low)
        |
        (
            static_cast<uint16_t>(high)
            << 8
        );


    return static_cast<int16_t>(
        value
    );
}