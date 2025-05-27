/**
 * @file
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
 * @brief Listar particiones de discos duros MBR/GPT
 * @copyright MIT License
*/

#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "mbr.h"
#include "gpt.h"

/**
* @brief Hex dumps a buffer
* @param buf Pointer to buffer
* @param size Buffer size
*/
void hex_dump(char * buf, size_t size);

/**
* @brief ASCII dumps a buffer
* @param buf Pointer to buffer
* @param size Buffer size
*/
void ascii_dump(char * buf, size_t size);



int main(int argc, char *argv[]) {
        // Leer disco
        mbr disco_mbr;
        char *direccion;
        FILE *disco;
        //En caso de no ingresar ningun prametro, o mas de los requeridos finalizar la ejecucion
        if(argc!=2){
          printf("Ingresar como parametro el disco");
          exit(1);
        }
        //Leer ubicación del archivo
        direccion=argv[1];
        disco=fopen(direccion,"rb");
        fread(&disco_mbr,sizeof(mbr),1,disco);
        fclose(disco);
        //Verificar si es un MBR valido y imprimir la información de las particiones
        if(is_mbr (&disco_mbr)){
            imprimir_mbr(&disco_mbr);
        }
        
        // Verificar si es un MBR de protección GPT
        if (!is_protective_mbr(&disco_mbr)) {
          return 0;
        }
        
        // Leer el encabezado GPT (sector 1)
       gpt_header hdr;
       FILE *disco_gpt = fopen(direccion, "rb");
       if (!disco_gpt) {
           perror("Error al abrir el disco para GPT");
           return 1;
       }
       fseek(disco_gpt, 512, SEEK_SET); // Ir al LBA 1
        fread(&hdr, sizeof(gpt_header), 1, disco_gpt);
        fclose(disco_gpt);

        // Validar encabezado GPT
        if (!is_valid_gpt_header(&hdr)) {
            printf("Encabezado GPT no válido.\n");
           return 1;
        }
        
        imprimir_gpt_header(&hdr);
        imprimir_gpt_partitions(direccion, &hdr);
        
        
	return 0;
}


int read_lba_sector(char * disk, unsigned long long lba, char buf[512]) {

	/* TODO leer un sector de 512 bytes del disco especificado en el buffer */
	return 0;


}

void ascii_dump(char * buf, size_t size) {
	for (size_t i = 0; i < size; i++) {
		if (buf[i] >= 0x20 && buf[i] < 0x7F) {
			printf("%c", buf[i]);
		}else {
			printf(".");
		}
	}
}

void hex_dump(char * buf, size_t size) {
	int cols;
	cols = 0;
	for (size_t i=0; i < size; i++) {
		printf("%02x ", buf[i] & 0xff);
		if (++cols % 16 == 0) {
			ascii_dump(&buf[cols - 16], 16);
			printf("\n");
		}
	}
}



