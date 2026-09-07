#include "wled.h"
#include "bus_manager.h"

class HexTuneOwnership : public Usermod
{
private:
    static constexpr uint8_t OWNER_PIN = 8;

    bool m_lastRemoteOwnership = false;
    bool m_initialized = false;

    void releaseWLED()
    {
        // Stop WLED from updating the LED strip.
        // Do NOT manipulate GPIO5 directly.
        strip.suspend();
        BusManager::off();

        DEBUG_PRINTLN(F("HexTune Ownership: D1 owns LED strip"));
    }

    void acquireWLED()
    {
        // Resume the existing WLED LED bus.
        // Do NOT remove, recreate, or reconfigure the bus.
        strip.resume();
        strip.trigger();

        DEBUG_PRINTLN(F("HexTune Ownership: S3/WLED owns LED strip"));
    }

    void applyOwnership(bool remoteOwnership)
    {
        if (remoteOwnership)
        {
            acquireWLED();
        }
        else
        {
            releaseWLED();
        }
    }

public:

    void setup() override
    {
        pinMode(OWNER_PIN, INPUT_PULLDOWN);

        const bool remoteOwnership =
            (digitalRead(OWNER_PIN) == HIGH);

        m_lastRemoteOwnership = remoteOwnership;
        m_initialized = true;

        DEBUG_PRINT(F("HexTune Ownership: GPIO8 = "));
        DEBUG_PRINTLN(
            remoteOwnership ? F("HIGH - S3/WLED")
                            : F("LOW - D1")
        );

        applyOwnership(remoteOwnership);
    }

    void loop() override
    {
        if (!m_initialized)
            return;

        const bool remoteOwnership =
            (digitalRead(OWNER_PIN) == HIGH);

        if (remoteOwnership == m_lastRemoteOwnership)
            return;

        m_lastRemoteOwnership = remoteOwnership;

        DEBUG_PRINT(F("HexTune Ownership: GPIO8 changed to "));
        DEBUG_PRINTLN(
            remoteOwnership ? F("HIGH - S3/WLED")
                            : F("LOW - D1")
        );

        applyOwnership(remoteOwnership);
    }

    uint16_t getId() override
    {
        return USERMOD_ID_UNSPECIFIED;
    }
};

static HexTuneOwnership hexTuneOwnership;

REGISTER_USERMOD(hexTuneOwnership);
