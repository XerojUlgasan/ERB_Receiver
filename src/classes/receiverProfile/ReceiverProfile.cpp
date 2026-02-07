#include <Arduino.h>

#include "ReceiverProfile.h"


ReceiverProfile::ReceiverProfile(Preferences& p) : pref(p)
{
}

ReceiverProfile::~ReceiverProfile()
{
}

void ReceiverProfile::setReceiverProfile(String fn, String bd, String cn, String rg, String ct, String bg){
    this->fullname = fn;
    this->birthdate = bd;
    this->contact_number = cn;
    this->region = rg;
    this->contact_number = ct;
    this->baranggay = bg;

    this->pref.begin("sender_profile");

    this->pref.putString("fullname", this->fullname);
    this->pref.putString("birthdate", this->birthdate);
    this->pref.putString("contact_number", this->contact_number);
    this->pref.putString("region", this->region);
    this->pref.putString("city", this->city);
    this->pref.putString("baranggay", this->baranggay);

    this->pref.end();

    return;
}