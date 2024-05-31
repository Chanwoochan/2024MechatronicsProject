#ifndef W_SET
#define W_SET

int wifi_set(const char* ssid, const char* password);

void httpInit();
void wather();

void soundBegin(byte baudrate);
void specify_Volume(byte level);
void specify_Track(int16_t track);
void execute_CMD(byte CMD, byte Par1, byte Par2);
void soundplay(int n);

void servoInit(int pin);
void servoStateUpdate();
void IRAM_ATTR servo();

void ledOFF();
void ledR();
void ledG();
void ledB();
void ledW();

double usingLPF();
void outLedNBuzzer(double distance);

#endif
