#ifndef W_SET
#define W_SET

int wifi_set(const char* ssid, const char* password);

void httpInit();
void digit(double num);
void min_digit(int Min);
void wather();
void times();

void soundBegin(byte baudrate);
void specify_Volume(byte level);
void specify_Track(int16_t track);
void playfTrack(int8_t folder,int8_t track);
void execute_CMD(byte CMD, byte Par1, byte Par2);
void soundplay(int8_t n);
void soundplay2(int8_t n);

void servoInit(int pin);
void servoStateUpdate();
void servo();

void ledOFF();
void ledRGB(int R, int G, int B);
void ledR();
void ledG();
void ledB();
void ledW();

void MPU6050Init();
void get6050();

double usingLPF();
void outLedNBuzzer(double distance);

#endif
