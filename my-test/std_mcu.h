#ifndef __MPUIRQ__
#define __MPUIRQ__
int shmcu_get_bat_volt(unsigned char volt_info[]) ;
int shmcu_get_bat_perc(unsigned char perc_info[]) ;
int shmcu_get_bat_curr(unsigned char curr_info[]);
int shmcu_get_bat_remain(unsigned char remain_info[]);
int shmcu_get_bat_full(unsigned char full_info[]);
int shmcu_check_function(void) ;

int shmcu_get_rtc_time(unsigned char rtc_time_info[]);

int shmcu_set_rtc_time(unsigned char rtc_time_info[]);

int shmcu_update_mode(unsigned char *mcu_mode);

int shmcu_update_read(unsigned char mcu_update_info[]);

int shmcu_update_write(unsigned char mcu_update_info[]);

int shmcu_command_send(unsigned char *dataBuffer, int count);

int shmcu_command_recv(unsigned char cmd, unsigned char *dataBuffer, int count);
int shmcu_host_status(int type) ;

int shmcu_get_board_id(void);
int shmcu_read_common_board_id(int nIdx, unsigned char *pId, int len);
void shmcu_write_board_id(unsigned char *pId, int id_len);
void shmcu_write_common_board_id(unsigned char *pId, int id_len);

int shmcu_read_mcu_snblk(char *pBlk);

#endif

