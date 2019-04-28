#include "L3_Application/periodic_scheduler.hpp"
#include "utility/log.hpp"

/// Toggles the led every 1s
void Print1Hz(uint32_t)
{
  static int counter = 0;
  printf("1 Hz counter: %d\n", counter++);
}
/// Toggles the led every 0.1s
void Print10Hz(uint32_t)
{
  static int counter = 0;
  printf("10 Hz counter: %d\n", counter++);
}

rtos::PeriodicScheduler scheduler = rtos::PeriodicScheduler();
rtos::PeriodicTask<512> blinker_1_hz_task("Print1Hz", rtos::Priority::kLow,
                                          Print1Hz);
rtos::PeriodicTask<512> blinker_10_hz_task("Print10Hz", rtos::Priority::kLow,
                                           Print10Hz);

int main()
{
  LOG_INFO("Starting PeriodicScheduler example...");
  scheduler.SetTask(&blinker_1_hz_task,
                    rtos::PeriodicScheduler::Frequency::k1Hz);
  scheduler.SetTask(&blinker_10_hz_task,
                    rtos::PeriodicScheduler::Frequency::k10Hz);
  rtos::TaskScheduler::Instance().Start();
  return 0;
}
