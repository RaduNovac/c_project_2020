#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ip_lib.h"
#include "bmp.h"

int main(){
    ip_mat *nuova, *copia, *sottomatrice, *i1, *i2, *i3, *i4, *i5, *i6, *i7, *i8, *i9;
    /*Creo matrice*/
    nuova = ip_mat_create(5,5,3,4.2);
    printf("\nip_mat creata\n");
    /*La inizializzo a random*/
    /*ip_mat_init_random(nuova,3,2);
    printf("\nip_mat inizializzata random\n");*/

    printf("\nNuova:\n");
    ip_mat_show(nuova);
    compute_stats(nuova);
    printf("\nStatistiche compilate\n");
    ip_mat_show_stats(nuova);

    /*Copio nuova*/
    copia = ip_mat_copy(nuova);
    printf("\nCopiata nuova in copia\n");

    printf("\nCopia:\n");
    ip_mat_show(copia);
    printf("\nStatistiche compilate\n");
    ip_mat_show_stats(copia);

    /*Creo sottomatrice*/

    /*La stampo a video*/
    sottomatrice = ip_mat_subset(nuova,1,4,1,4);
    printf("\nSottomatrice creata\n");
    printf("\nSottomatrice è un subset di nuova, da riga 1 a 4 e colonna 1 a 4\n");
    ip_mat_show(sottomatrice);
    ip_mat_show_stats(sottomatrice);

    /*Concatenazione di matrici*/
    /*Da i1 a i3 le matrici che userò per avere in output da i4 a i6*/

    i1 = ip_mat_create(5,3,3,3.2);
    i2 = ip_mat_create(4,3,3,4.5);

    i4 = ip_mat_create(3,5,3,1.2);
    i5 = ip_mat_create(3,4,3,6.5);

    i7 = ip_mat_create(3,3,2,2.3);
    i8 = ip_mat_create(3,3,4,7.1);

    /*Le concateno*/
    /*Concateno asse x*/
    printf("\nConcateno i1 e i2 su asse x e dovrei avere una ip_mat 9x3x3\n");

    i3 = ip_mat_concat(i1,i2,0);
    printf("\ni1 e i2 concatenate su asse x\n");
    ip_mat_show(i3);
    ip_mat_show_stats(i3);

    /*Concateno asse y*/

    i6 = ip_mat_concat(i4,i5,1);
    printf("\ni4 e i5 concatenate su asse y\n");
    ip_mat_show(i6);
    ip_mat_show_stats(i6);

    /*Concateno sui canali*/

    i9 = ip_mat_concat(i7,i8,2);
    printf("\ni7 e i8 concatenate su asse dei canali (z)\n");
    ip_mat_show(i9);
    ip_mat_show_stats(i9);

    /*Libero tutte le ip_mat*/
    ip_mat_free(nuova);
    ip_mat_free(copia);
    ip_mat_free(sottomatrice);
    ip_mat_free(i1);
    ip_mat_free(i2);
    ip_mat_free(i3);
    ip_mat_free(i4);
    ip_mat_free(i5);
    ip_mat_free(i6);
    ip_mat_free(i7);
    ip_mat_free(i8);
    ip_mat_free(i9);
    printf("\nip_mat liberate\n");

    return 0;
}