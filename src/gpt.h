/**
* @file
* @author Erwin Meza Vega <emezav@unicauca.edu.co>
* @brief Definiciones para discos inicializados con esquema GPT
* @copyright MIT License
*/

#ifndef GPT_H
#define GPT_H

#include "mbr.h"

/**
* @brief GUID
* @see https://uefi.org/specs/UEFI/2.10/Apx_A_GUID_and_Time_Formats.html
*/
typedef struct {
	unsigned int time_lo; /*!< Low field of the timestamp */
	unsigned short time_mid; /*!< Middle field of the timestamp */
	unsigned short time_hi_and_version; /*!< High field of the timestamp and version */
	unsigned char clock_seq_hi_and_reserved; /*!< High field of the clock sequence */
	unsigned char clock_seq_lo; /*!< Low field of the clock sequence */
	unsigned char node[6]; /*!< Spatially unique node identifier */
}__attribute__((packed))guid;

/** @brief GPT Partition Table Header. */
typedef struct {
    char signature[8];                // "EFI PART"
    unsigned int revision;           // Revisión
    unsigned int header_size;        // Tamaño del encabezado
    unsigned int crc32_header;       // Checksum del encabezado
    unsigned int reserved;           // Reservado (debe ser 0)
    unsigned long long current_lba;  // LBA del encabezado
    unsigned long long backup_lba;   // LBA de respaldo
    unsigned long long first_usable_lba;
    unsigned long long last_usable_lba;
    guid disk_guid;                  // GUID del disco
    unsigned long long partition_entry_lba;   // LBA de entrada de la tabla
    unsigned int partition_entries;           // Número de entradas
    unsigned int partition_entry_size;        // Tamaño de cada entrada
    unsigned int crc32_partition_array;       // Checksum de la tabla de particiones
}__attribute__((packed)) gpt_header;

/**
* @brief GPT Partition Entry
*/
typedef struct {
    guid partition_type_guid;      // Tipo de partición
    guid unique_partition_guid;    // GUID único de la partición
    unsigned long long start_lba;  // LBA inicial
    unsigned long long end_lba;    // LBA final
    unsigned long long attributes; // Atributos
    char partition_name[72];       // Nombre (UTF-16LE)
}__attribute__((packed)) gpt_partition_descriptor;

/**
* @brief GPT Partition type
*/
typedef struct {
	const char * os; /*!< Operating system */
	const char * description; /*!< Description */
	const char * guid; /*!< GUID */
}gpt_partition_type;

/**
* @brief Text description of a GPT partition type
* @param type Partition type reported in MBR
* @param buf String buffer to store the text description
*/
const gpt_partition_type* get_gpt_partition_type(char * guid_str);

/**
* @brief Decodes a two-byte encoded partition name
* @param name two-byte encoded partition name
*/
char * gpt_decode_partition_name(char name[72]);

/**
* @brief Checks if a bootsector is Protective MBR.
* @param boot_record Bootsector read in memory]
* @return 1 If the bootsector is a Protective MBR, 0 otherwise.
*/
int is_protective_mbr(mbr * boot_record);

/**
* @brief Checks if a GPT header is valid.
* @param hdr Pointer to the GPT header
* @return 1 of hdr is a valid GPT header, 0 otherwise.
*/
int is_valid_gpt_header(gpt_header * hdr);


/**
* @brief Checks if the GPT partition descriptor is null (not used)
* @param desc Descriptor
* @return 1 if the descriptor is null (partition_type_guid = 0), 0 otherwise.
*/
int is_null_descriptor(gpt_partition_descriptor * desc);


/**
* @brief Creates a human-readable representation of a GUID
* @param buf Buffer containing the GUID
* @return New string with the text representation of the GUID
*/
char * guid_to_str(guid * buf);

void imprimir_gpt_header(gpt_header * hdr);

void imprimir_gpt_partitions(const char * ruta, gpt_header * hdr);

#endif
