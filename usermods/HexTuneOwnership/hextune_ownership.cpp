#include "wled.h"

class HexTuneOwnership : public Usermod
{
private:
    static constexpr uint8_t OWNER_PIN = 8;

    bool m_lastState = false;
    bool m_initialized = false;

public:

    void setup() override
    {
        pinMode(OWNER_PIN, INPUT_PULLDOWN);

        m_lastState = (digitalRead(OWNER_PIN) == HIGH);
        m_initialized = true;

        DEBUG_PRINT(F("HexTune Ownership GPIO8 = "));
        DEBUG_PRINTLN(m_lastState ? F("HIGH") : F("LOW"));
    }

    void loop() override
    {
        if (!m_initialized)
            return;

        const bool state = (digitalRead(OWNER_PIN) == HIGH);

        if (state == m_lastState)
            return;

        m_lastState = state;

        DEBUG_PRINT(F("HexTune Ownership GPIO8 changed = "));
        DEBUG_PRINTLN(state ? F("HIGH") : F("LOW"));
    }

    void addToJsonInfo(JsonObject& root) override
    {
        JsonObject user = root["u"].to<JsonObject>();
        user["HexTune GPIO8"] = m_lastState ? "HIGH" : "LOW";
    }

    uint16_t getId() override
    {
        return USERMOD_ID_UNSPECIFIED;
    }
};

static HexTuneOwnership hexTuneOwnership;

REGISTER_USERMOD(hexTuneOwnership);
