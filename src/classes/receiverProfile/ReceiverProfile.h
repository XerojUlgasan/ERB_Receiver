#include <Arduino.h>
#include <Preferences.h>

enum ResponderType{
    TANOD,          //0
    POLICE,         //1
    MEDIC,          //2
    FIREFIGHTER     //3
};

class ReceiverProfile
{
private:
    Preferences& pref;
public:
    String fullname, birthdate, address, contact_number, region, city, baranggay;
    uint8_t responder_type;

    ReceiverProfile(Preferences& pref);
    ~ReceiverProfile();

    void setReceiverProfile(String fn, String bd, String cn, String rg, String ct, String bg);
};
