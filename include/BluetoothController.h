#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>

#include <atomic>
#include <cstdint>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "ControlModule.h"
#include "ControllerInput.h"


class BluetoothController : public ControlModule
{

public:

    BluetoothController();

    void setup() override;
    void loop() override;
    void stop() override;

    bool isConnected() const;

    bool hasTimedOut(
        uint32_t timeoutMs = 300
    ) const;

    const ControllerInput& getInput() const;


private:

    // --------------------
    // C7 BLE info
    // --------------------

    static constexpr const char* DEVICE_NAME =
        "C7-RC";

    static constexpr const char* SERVICE_UUID =
        "c7000001-7c7c-4c7c-8c7c-c7c7c7c7c7c7";

    static constexpr const char* CONTROL_UUID =
        "c7000002-7c7c-4c7c-8c7c-c7c7c7c7c7c7";

    static constexpr size_t CONTROL_PACKET_SIZE = 10;


    // --------------------
    // BLE objects
    // --------------------

    NimBLEServer* server = nullptr;

    NimBLECharacteristic* controlCharacteristic = nullptr;


    // --------------------
    // Controller state
    // --------------------

    ControllerInput currentInput;

    QueueHandle_t inputQueue = nullptr;

    std::atomic<bool> connected{false};

    uint32_t lastPacketMs = 0;


    // --------------------
    // BLE callbacks
    // --------------------

    class ServerCallbacks :
        public NimBLEServerCallbacks
    {

    public:

        explicit ServerCallbacks(
            BluetoothController& parent
        );


        void onConnect(
            NimBLEServer* server,
            NimBLEConnInfo& connInfo
        ) override;


        void onDisconnect(
            NimBLEServer* server,
            NimBLEConnInfo& connInfo,
            int reason
        ) override;


    private:

        BluetoothController& parent;
    };


    class ControlCallbacks :
        public NimBLECharacteristicCallbacks
    {

    public:

        explicit ControlCallbacks(
            BluetoothController& parent
        );


        void onWrite(
            NimBLECharacteristic* characteristic,
            NimBLEConnInfo& connInfo
        ) override;


    private:

        BluetoothController& parent;
    };


    ServerCallbacks serverCallbacks;
    ControlCallbacks controlCallbacks;


    // --------------------
    // Packet decoding
    // --------------------

    void handleControlPacket(
        const uint8_t* data,
        size_t length
    );


    static int16_t decodeInt16LE(
        uint8_t low,
        uint8_t high
    );
};