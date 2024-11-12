
#ifndef FLEXCAN_IRQ_H
#define FLEXCAN_IRQ_H

#include "device_registers.h"

void FLEXCAN_IRQHandler(uint8_t instance);

void FLEXCAN_Error_IRQHandler(uint8_t instance);

#if FEATURE_CAN_HAS_WAKE_UP_IRQ

void FLEXCAN_WakeUpHandler(uint8_t instance);

#endif 

#if (defined(CPU_S32K116) || (defined(CPU_S32K118)))

void CAN0_ORed_Err_Wakeup_IRQHandler(void);
void CAN0_ORed_0_31_MB_IRQHandler(void);

#elif (defined(CPU_S32K142) || defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT) || \
     defined(CPU_S32K146) || defined(CPU_S32K148) || defined(CPU_S32MTV) || defined(CPU_S32K116)\
     || (defined(CPU_S32K118)))

void CAN0_ORed_IRQHandler(void);
void CAN0_Error_IRQHandler(void);
#if FEATURE_CAN_HAS_WAKE_UP_IRQ
void CAN0_Wake_Up_IRQHandler(void);
#endif
void CAN0_ORed_0_15_MB_IRQHandler(void);
void CAN0_ORed_16_31_MB_IRQHandler(void);

#if (CAN_INSTANCE_COUNT > 1U)

void CAN1_ORed_IRQHandler(void);
void CAN1_Error_IRQHandler(void);
void CAN1_ORed_0_15_MB_IRQHandler(void);
void CAN1_ORed_16_31_MB_IRQHandler(void);

#endif 

#if (CAN_INSTANCE_COUNT > 2U)

void CAN2_ORed_IRQHandler(void);
void CAN2_Error_IRQHandler(void);
void CAN2_ORed_0_15_MB_IRQHandler(void);
void CAN2_ORed_16_31_MB_IRQHandler(void);

#endif 

#elif defined(CPU_S32V234)

void CAN0_IRQHandler(void);
void CAN0_Buff_IRQHandler(void);

#if (CAN_INSTANCE_COUNT > 1U)

void CAN1_IRQHandler(void);
void CAN1_Buff_IRQHandler(void);

#endif 

#elif (defined(CPU_MPC5748G) || defined(CPU_MPC5746C) || defined(CPU_MPC5744B) || \
       defined(CPU_MPC5745B) || defined(CPU_MPC5746B) || defined(CPU_MPC5744C) || \
       defined(CPU_MPC5745C) || defined(CPU_MPC5747C) || defined(CPU_MPC5748C) || \
       defined(CPU_MPC5746G) || defined(CPU_MPC5747G))

void CAN0_ORed_IRQHandler(void);
void CAN0_Error_IRQHandler(void);
#if FEATURE_CAN_HAS_WAKE_UP_IRQ
void CAN0_Wake_Up_IRQHandler(void);
#endif
void CAN0_ORed_00_03_MB_IRQHandler(void);
void CAN0_ORed_04_07_MB_IRQHandler(void);
void CAN0_ORed_08_11_MB_IRQHandler(void);
void CAN0_ORed_12_15_MB_IRQHandler(void);
void CAN0_ORed_16_31_MB_IRQHandler(void);
void CAN0_ORed_32_63_MB_IRQHandler(void);
void CAN0_ORed_64_95_MB_IRQHandler(void);

#if (CAN_INSTANCE_COUNT > 1U)

void CAN1_ORed_IRQHandler(void);
void CAN1_Error_IRQHandler(void);
void CAN1_ORed_00_03_MB_IRQHandler(void);
void CAN1_ORed_04_07_MB_IRQHandler(void);
void CAN1_ORed_08_11_MB_IRQHandler(void);
void CAN1_ORed_12_15_MB_IRQHandler(void);
void CAN1_ORed_16_31_MB_IRQHandler(void);
void CAN1_ORed_32_63_MB_IRQHandler(void);
void CAN1_ORed_64_95_MB_IRQHandler(void);

#endif 

#if (CAN_INSTANCE_COUNT > 2U)

void CAN2_ORed_IRQHandler(void);
void CAN2_Error_IRQHandler(void);
void CAN2_ORed_00_03_MB_IRQHandler(void);
void CAN2_ORed_04_07_MB_IRQHandler(void);
void CAN2_ORed_08_11_MB_IRQHandler(void);
void CAN2_ORed_12_15_MB_IRQHandler(void);
void CAN2_ORed_16_31_MB_IRQHandler(void);
void CAN2_ORed_32_63_MB_IRQHandler(void);
void CAN2_ORed_64_95_MB_IRQHandler(void);

#endif 

#if (CAN_INSTANCE_COUNT > 3U)

void CAN3_ORed_IRQHandler(void);
void CAN3_Error_IRQHandler(void);
void CAN3_ORed_00_03_MB_IRQHandler(void);
void CAN3_ORed_04_07_MB_IRQHandler(void);
void CAN3_ORed_08_11_MB_IRQHandler(void);
void CAN3_ORed_12_15_MB_IRQHandler(void);
void CAN3_ORed_16_31_MB_IRQHandler(void);
void CAN3_ORed_32_63_MB_IRQHandler(void);
void CAN3_ORed_64_95_MB_IRQHandler(void);

#endif

#if (CAN_INSTANCE_COUNT > 4U)

void CAN4_ORed_IRQHandler(void);
void CAN4_Error_IRQHandler(void);
void CAN4_ORed_00_03_MB_IRQHandler(void);
void CAN4_ORed_04_07_MB_IRQHandler(void);
void CAN4_ORed_08_11_MB_IRQHandler(void);
void CAN4_ORed_12_15_MB_IRQHandler(void);
void CAN4_ORed_16_31_MB_IRQHandler(void);
void CAN4_ORed_32_63_MB_IRQHandler(void);
void CAN4_ORed_64_95_MB_IRQHandler(void);

#endif 

#if (CAN_INSTANCE_COUNT > 5U)

void CAN5_ORed_IRQHandler(void);
void CAN5_Error_IRQHandler(void);
void CAN5_ORed_00_03_MB_IRQHandler(void);
void CAN5_ORed_04_07_MB_IRQHandler(void);
void CAN5_ORed_08_11_MB_IRQHandler(void);
void CAN5_ORed_12_15_MB_IRQHandler(void);
void CAN5_ORed_16_31_MB_IRQHandler(void);
void CAN5_ORed_32_63_MB_IRQHandler(void);
void CAN5_ORed_64_95_MB_IRQHandler(void);

#endif
#if (CAN_INSTANCE_COUNT > 6U)

void CAN6_ORed_IRQHandler(void);
void CAN6_Error_IRQHandler(void);
void CAN6_ORed_00_03_MB_IRQHandler(void);
void CAN6_ORed_04_07_MB_IRQHandler(void);
void CAN6_ORed_08_11_MB_IRQHandler(void);
void CAN6_ORed_12_15_MB_IRQHandler(void);
void CAN6_ORed_16_31_MB_IRQHandler(void);
void CAN6_ORed_32_63_MB_IRQHandler(void);
void CAN6_ORed_64_95_MB_IRQHandler(void);

#endif 

#if (CAN_INSTANCE_COUNT > 7U)

void CAN7_ORed_IRQHandler(void);
void CAN7_Error_IRQHandler(void);
void CAN7_ORed_00_03_MB_IRQHandler(void);
void CAN7_ORed_04_07_MB_IRQHandler(void);
void CAN7_ORed_08_11_MB_IRQHandler(void);
void CAN7_ORed_12_15_MB_IRQHandler(void);
void CAN7_ORed_16_31_MB_IRQHandler(void);
void CAN7_ORed_32_63_MB_IRQHandler(void);
void CAN7_ORed_64_95_MB_IRQHandler(void);

#endif 

#elif (defined(CPU_MPC5741P) || defined(CPU_MPC5742P) || defined(CPU_MPC5743P) || \
       defined(CPU_MPC5744P) || defined(CPU_S32R372) || defined(CPU_S32R274))

void CAN0_ORed_IRQHandler(void);
void CAN0_Error_IRQHandler(void);
#if FEATURE_CAN_HAS_WAKE_UP_IRQ
void CAN0_Wake_Up_IRQHandler(void);
#endif
void CAN0_ORed_00_03_MB_IRQHandler(void);
void CAN0_ORed_04_07_MB_IRQHandler(void);
void CAN0_ORed_08_11_MB_IRQHandler(void);
void CAN0_ORed_12_15_MB_IRQHandler(void);
void CAN0_ORed_16_31_MB_IRQHandler(void);
void CAN0_ORed_32_39_MB_IRQHandler(void);
void CAN0_ORed_40_47_MB_IRQHandler(void);
void CAN0_ORed_48_55_MB_IRQHandler(void);
void CAN0_ORed_56_63_MB_IRQHandler(void);
#if (defined(CPU_S32R372) || defined(CPU_S32R274))
void CAN0_ORed_64_79_MB_IRQHandler(void);
void CAN0_ORed_80_95_MB_IRQHandler(void);
#endif

#if (CAN_INSTANCE_COUNT > 1U)

#if( defined(CPU_MPC5741P) || defined(CPU_MPC5742P) || defined(CPU_MPC5743P) || \
     defined(CPU_MPC5744P) || defined(CPU_S32R274))
void CAN1_ORed_IRQHandler(void);
void CAN1_Error_IRQHandler(void);
void CAN1_ORed_00_03_MB_IRQHandler(void);
void CAN1_ORed_04_07_MB_IRQHandler(void);
void CAN1_ORed_08_11_MB_IRQHandler(void);
void CAN1_ORed_12_15_MB_IRQHandler(void);
void CAN1_ORed_16_31_MB_IRQHandler(void);
void CAN1_ORed_32_39_MB_IRQHandler(void);
void CAN1_ORed_40_47_MB_IRQHandler(void);
void CAN1_ORed_48_55_MB_IRQHandler(void);
void CAN1_ORed_56_63_MB_IRQHandler(void);
#else 
void CAN2_ORed_IRQHandler(void);
void CAN2_Error_IRQHandler(void);
void CAN2_ORed_00_03_MB_IRQHandler(void);
void CAN2_ORed_04_07_MB_IRQHandler(void);
void CAN2_ORed_08_11_MB_IRQHandler(void);
void CAN2_ORed_12_15_MB_IRQHandler(void);
void CAN2_ORed_16_31_MB_IRQHandler(void);
void CAN2_ORed_32_39_MB_IRQHandler(void);
void CAN2_ORed_40_47_MB_IRQHandler(void);
void CAN2_ORed_48_55_MB_IRQHandler(void);
void CAN2_ORed_56_63_MB_IRQHandler(void);
void CAN2_ORed_64_79_MB_IRQHandler(void);
void CAN2_ORed_80_95_MB_IRQHandler(void);
#endif 
#endif 

#if (CAN_INSTANCE_COUNT > 2U)

void CAN2_ORed_IRQHandler(void);
void CAN2_Error_IRQHandler(void);
void CAN2_ORed_00_03_MB_IRQHandler(void);
void CAN2_ORed_04_07_MB_IRQHandler(void);
void CAN2_ORed_08_11_MB_IRQHandler(void);
void CAN2_ORed_12_15_MB_IRQHandler(void);
void CAN2_ORed_16_31_MB_IRQHandler(void);
void CAN2_ORed_32_39_MB_IRQHandler(void);
void CAN2_ORed_40_47_MB_IRQHandler(void);
void CAN2_ORed_48_55_MB_IRQHandler(void);
void CAN2_ORed_56_63_MB_IRQHandler(void);
#if (defined(CPU_S32R372) || defined(CPU_S32R274))
void CAN2_ORed_64_79_MB_IRQHandler(void);
void CAN2_ORed_80_95_MB_IRQHandler(void);
#endif

#endif 

#else
    #error "No valid CPU defined!"
#endif

#endif 


