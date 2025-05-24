/**
* @file
* @brief Definiciones para discos inicializados con esquema BR
*/

#ifndef MBR_H
#define MBR_H

#include <stdint.h>  

/** @brief Boot sector signature */
#define MBR_SIGNATURE 0xAA55

/** @brief GPT Partition type - MBR */
#define MBR_TYPE_GPT 0xEE

/** @brief Unused partition table - MBR*/
#define MBR_TYPE_UNUSED 0x00

/** @brief Longitud maxima del texto para el tipo de particion */
#define TYPE_NAME_LEN 256

/** @brief Partition descriptor - MBR */
typedef struct {
    uint8_t  estado_arranque;       // (1 byte)
    uint8_t  inicio_chs[3];    // (3 bytes)
    uint8_t  tipo_particion;  // (1 byte)
    uint8_t  fin_chs[3];      // (3 bytes)
    uint32_t inicio_lba;       // (4 bytes)
    uint32_t tamano_particion;    // (4 bytes)
}__attribute__((packed)) mbr_partition_descriptor;

/** @brief Master Boot Record. */
typedef struct {
    uint8_t bootstrap[446];    // (446 bytes)
    mbr_partition_descriptor particiones[4]; // Tabla de particiones (64 bytes)
    uint16_t arranque;       // (2 bytes)
}__attribute__((packed)) mbr;


/**
* @brief Checks if a bootsector is a MBR.
* @param boot_record Bootsector read in memory]
* @return 1 If the bootsector is a Protective MBR, 0 otherwise.
*/
int is_mbr(mbr * boot_record);

/**
* @brief Text description of a MBR partition type
* @param type Partition type reported in MBR
* @param buf String buffer to store the text description
*/
void mbr_partition_type(uint8_t  tipo);

//Imprimir MBR
void imprimir_mbr(mbr * boot_record);


#endif
