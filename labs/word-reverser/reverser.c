#include <stdio.h>
#include <stdlib.h>
#define ARRAY_LENGTH 100

void reverse(char arreglo[], int length){
	char tmp;
    for (int i = 0;  i < length/2; i++) {
		tmp = arreglo[i];
		arreglo[i] = arreglo[length - i - 1];
		arreglo[length - i - 1] = tmp;
    }
	printf("%s", arreglo);
}

int main(int argc, char **argv){
	int c;
	char arreglo[ARRAY_LENGTH];
	int contador=0;
    while ((c = getchar()) != EOF) {
    	if (c == '\n'){ 
       		reverse(arreglo,contador);
			for(int i = 0 ; i < contador ; i++){
				arreglo[i] = '\0';
			}
			contador=0; 
    	}else{
    		arreglo[contador] = c;
    		contador++;
    	}
    }
	return 0;
}