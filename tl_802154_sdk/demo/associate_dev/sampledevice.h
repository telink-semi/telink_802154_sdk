#ifndef _sampledevice_H_
#define _sampledevice_H_


void led_init(void);
/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention);
//deep
extern void app_task(void);


void sampleDeviceSysException(void);
#endif /* _MAC_TEST_H_ */
