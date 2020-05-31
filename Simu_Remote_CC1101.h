//by Little_Satan

#define Simu_Remote_CC1101_h
#include <Arduino.h>


class Simu
{
private:
void convert_bit(void);
void decrypt(void);
void check_crc(void);
void btn_repeat(void);
static void handleInterrupt();

void set_crc_encrypt(void);
void prg_transmit(int rem,int btn);
void transmit_init(void);
void transmit_sync1(void);
void transmit_sync2(int rep);
void transmit_code(void);
void esp_eeprom_begin(void);
void esp_eeprom_commit(void);

public:
void Init(void);
void enableReceive(int pin);
void disableReceive(void);
void enableTransmit(int pin);

void add_remote(uint8_t rc0,uint8_t rc1,uint8_t btn_id,uint8_t rem_id1,uint8_t rem_id2,int rem,int btn);
void prg_transmit(void);
void btn_transmit(int rem,int btn,int hwb);
void transmit(int rem,int btn,int rep);
void transmit(uint8_t rc0,uint8_t rc1,uint8_t btn_id,uint8_t rem_id1,uint8_t rem_id2,int rep);

bool receive(void);
int received_code(int addr, bool c);
void clear_received(void);
};

extern Simu simu;
