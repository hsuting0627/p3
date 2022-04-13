#include "mbed.h"
#include "stm32l475e_iot01_accelero.h"
#include <cmath>

InterruptIn btnRecord(BUTTON1);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
DigitalOut led(LED1);
Thread t;
#define X (0)
#define Y (0)
#define Z (1017)
#define A (1017)
int16_t pDataXYZ[3] = {0};
//int idR[32] = {0};
int idR;
int indexR = 0;
int B;
double angle = 0;
void record(void) {
   BSP_ACCELERO_AccGetXYZ(pDataXYZ);
    led = !led;
  // printf("%d, %d, %d\n", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
}

void startRecord(void) {
   idR = queue.call_every(1ms, record);
   //idR[indexR++] = queue.call_every(1ms, record);
   //indexR = indexR % 32;
}

void stopRecord(void) {
   queue.cancel(idR);
   //for (auto &i : idR)
   //   queue.cancel(i);
}

int main() {
   printf("Start accelerometer init\n");
   BSP_ACCELERO_Init();
   t.start(callback(&queue, &EventQueue::dispatch_forever));
   btnRecord.fall(queue.event(startRecord));
   btnRecord.rise(queue.event(stopRecord));
   while(1)
   {
       for (int i = 0; i <10 ; i++) {
       B = sqrt((pDataXYZ[0]*pDataXYZ[0])+(pDataXYZ[1]*pDataXYZ[1])+(pDataXYZ[2]*pDataXYZ[2]));
       angle = acos( pDataXYZ[2]*1017.0 / 1017.0 / B);
       }
       printf("%f\n", angle * 57.3);
       ThisThread::sleep_for(100);
   }
}