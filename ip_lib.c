/*
 Created by Sebastiano Vascon on 23/03/20.
*/

#include <stdio.h>
#include "ip_lib.h"
#include "bmp.h"

void ip_mat_show(ip_mat * t){
    unsigned int i,l,j;
    printf("Matrix of size %d x %d x %d (hxwxk)\n",t->h,t->w,t->k);
    for (l = 0; l < t->k; l++) {
        printf("Slice %d\n", l);
        for(i=0;i<t->h;i++) {
            for (j = 0; j < t->w; j++) {
                printf("%f ", get_val(t,i,j,l));
            }
            printf("\n");
        }
        printf("\n");
    }
}

void ip_mat_show_stats(ip_mat * t){
    unsigned int k;

    compute_stats(t);

    for(k=0;k<t->k;k++){
        printf("Channel %d:\n", k);
        printf("\t Min: %f\n", t->stat[k].min);
        printf("\t Max: %f\n", t->stat[k].max);
        printf("\t Mean: %f\n", t->stat[k].mean);
    }
}

ip_mat * bitmap_to_ip_mat(Bitmap * img){
    unsigned int i=0,j=0;

    unsigned char R,G,B;

    unsigned int h = img->h;
    unsigned int w = img->w;

    ip_mat * out = ip_mat_create(h, w,3,0);

    for (i = 0; i < h; i++)              /* rows */
    {
        for (j = 0; j < w; j++)          /* columns */
        {
            bm_get_pixel(img, j,i,&R, &G, &B);
            set_val(out,i,j,0,(float) R);
            set_val(out,i,j,1,(float) G);
            set_val(out,i,j,2,(float) B);
        }
    }

    return out;
}

Bitmap * ip_mat_to_bitmap(ip_mat * t){

    Bitmap *b = bm_create(t->w,t->h);

    unsigned int i, j;
    for (i = 0; i < t->h; i++)              /* rows */
    {
        for (j = 0; j < t->w; j++)          /* columns */
        {
            bm_set_pixel(b, j,i, (unsigned char) get_val(t,i,j,0),
                    (unsigned char) get_val(t,i,j,1),
                    (unsigned char) get_val(t,i,j,2));
        }
    }
    return b;
}

float get_val(ip_mat * a, unsigned int i,unsigned int j,unsigned int k){
    if(i<a->h && j<a->w &&k<a->k){  /* j>=0 and k>=0 and i>=0 is non sense*/
        return a->data[i][j][k];
    }else{
        printf("Errore get_val!!!");
        exit(1);
    }
}

void set_val(ip_mat * a, unsigned int i,unsigned int j,unsigned int k, float v){
    if(i<a->h && j<a->w &&k<a->k){
        a->data[i][j][k]=v;
    }else{
        printf("Errore set_val!!!");
        exit(1);
    }
}

float get_normal_random(){
    float y1 = ((float)(rand()) + 1.)/((float)(RAND_MAX) + 1.);
    float y2 = ((float)(rand()) + 1.)/((float)(RAND_MAX) + 1.);
    return cos(2*PI*y2)*sqrt(-2.*log(y1));
}

/**************************************Nostre funzioni*************************************/
/*----------------------------------------------------------------------------------------*/
ip_mat * ip_mat_create(unsigned int h, unsigned int w,unsigned  int k, float v)
{
	ip_mat *new; /* Puntatore alla struttura da ritornare */
	int i, j, l;
	
	new = (ip_mat*) malloc(sizeof(ip_mat));
	
	new -> h = h;
	new -> w = w;
	new -> k = k;
	
	new -> stat = (stats *) malloc(k * sizeof(stats));/*Alloco il vettore di stats*/
	
	new -> data = (float ***) malloc(h * sizeof(float **));/*Alloco la matrice 3D*/
	
	for(i = 0; i < h; i++) /* Inizializzo le righe */
	{
		new -> data[i] = (float **) malloc(w * sizeof(float *));
		
		for(j = 0; j < w; j++) /* Inizializzo le colonne */
		{
			new -> data[i][j] = (float *) malloc(k * sizeof(float));
			
			for(l = 0; l < k; l++){ /* Inizializzo i vari canali al valore v */
				new -> data[i][j][l] = v;
            }
		}
	}
	
	return new;
}
/*----------------------------------------------------------------------------------------*/
/* Libera la memoria (data, stat e la struttura) */
void ip_mat_free(ip_mat *a){
    int i,j;

    for(i = 0; i < a -> h; i++){ /* Scorro le righe */
		for(j = 0; j < a -> w; j++){ /* Scorro le colonne */
            free(a -> data[i][j]);/*Libero le colonne*/
		}
        free(a -> data[i]);/*Libero le righe*/
	}

    free(a -> data);/*Libero l'array principale*/

    free(a -> stat);/*Libero stats*/

    free(a);
}
/*----------------------------------------------------------------------------------------*/
/* 
 * Calcola il valore minimo, il massimo e la media per ogni canale di tutta l'immagine
 * e li salva dentro la struttura stats di ip_mat  
 */

void compute_stats(ip_mat * t){
    float min, max, media;
    int i = 0,j = 0,l = 0;

    for(i = 0;i < t -> k;i++){/*Per ogni canale*/

        min = t -> data[0][0][i];
        max = t -> data[0][0][i];
        media = t -> data[0][0][i];

        for(j = 0;j < t -> h;j++){/*Scorro ogni riga*/
            for(l = 0;l < t -> w;l++){/*Scorro ogni colonna*/
                if(min > t -> data[j][l][i]){
                    min = t -> data[j][l][i];
                }
                if(max < t -> data[j][l][i]){
                    max = t -> data[j][l][i];
                }
                media = (media + (t -> data[j][l][i]))/2.0;
            }
        }

        t -> stat[i].min = min;
        t -> stat[i].max = max;
        t -> stat[i].mean = media;
    }
}
/*----------------------------------------------------------------------------------------*/
/* 
 * Inizializza una ip_mat con dimensioni w h e k.
 * Ogni elemento è generato da una gaussiana con media mean e varianza var
 * 
 * Per la formula ho guardato qui: https://it.wikipedia.org/wiki/Standardizzazione_(statistica)
 */
float casuale(float mean, float var){
	float res;

	res = get_normal_random();

	res = (res - mean) / var;/*Lo adatto alla media e alla varianza, non so se devo fare così*/

	return res;
}

void ip_mat_init_random(ip_mat * t, float mean, float var){
    int x,y,z;/*Le 3 dimensioni della matrice*/

    for(z = 0; z < t -> k;z++){/*Lo faccio per singolo canale*/
        for(x = 0;x < t ->h;x++){/*Scorro ogni riga*/
            for(y = 0;y < t -> w;y++){/*E colonna*/
                t -> data[x][y][z] = casuale(mean,var);
            }
        }
    }
}
/*----------------------------------------------------------------------------------------*/
/* Crea una copia di una ip_mat e lo restituisce in output */
ip_mat * ip_mat_copy(ip_mat * in){
    ip_mat *out;

    int x, y, z;
	
	out = (ip_mat*)malloc(sizeof(ip_mat));
	
	out -> h = in -> h;
	out -> w = in -> w;
	out -> k = in -> k;
	
	out -> stat = (stats *) malloc(in -> k * sizeof(stats));/*Alloco il vettore di stats*/
	
	out -> data = (float ***) malloc(in -> h * sizeof(float **));/*Alloco la matrice 3D*/
	
	for(x = 0; x < out -> h; x++) /* Inizializzo le righe */
	{
		out -> data[x] = (float **) malloc(in -> w * sizeof(float *));
		
		for(y = 0; y < in -> w; y++) /* Inizializzo le colonne */
		{
			out -> data[x][y] = (float *) malloc(in -> k * sizeof(float));
			
			for(z = 0; z < in -> k; z++){ /* Inizializzo i vari canali ai valori di in */
				out -> data[x][y][z] = in -> data[x][y][z];
            }
		}
	}

    compute_stats(out);
	
	return out;
}
/*----------------------------------------------------------------------------------------*/
/* Restituisce una sotto-matrice, ovvero la porzione individuata da:
 * t->data[row_start...row_end][col_start...col_end][0...k]
 * La terza dimensione la riportiamo per intero, stiamo in sostanza prendendo un sottoinsieme
 * delle righe e delle colonne.
 * */
ip_mat * ip_mat_subset(ip_mat * t, unsigned int row_start, unsigned int row_end, unsigned int col_start, unsigned int col_end){
    ip_mat *out;/*Sottomatrice che ritornerò*/
    int x,y,z;

    if(row_start < 0 || row_end > t -> h || col_start < 0 || col_end > t -> w){
        exit(1);/*Se vado oltre le dimensioni di t*/
    }

    out = ip_mat_create(row_end - row_start, col_end - col_start, t -> k, 0.0);/*La istanzio*/
    
    for(x = row_start; x < row_end; x++){
        for(y = col_start; y < col_end;y++){
            for(z = 0;z < t -> k;z++){/*Copio tutti i canali*/
                out -> data[x - row_start][y - col_start][z] = t -> data[x][y][z];
            }
        }
    }

    compute_stats(out);/*Riempio le statistiche*/

    return out;
}
/*----------------------------------------------------------------------------------------*/
/* Concatena due ip_mat su una certa dimensione.
 * Ad esempio:
 * ip_mat_concat(ip_mat * a, ip_mat * b, 0);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h + b.h
 *      out.w = a.w = b.w
 *      out.k = a.k = b.k
 *
 * ip_mat_concat(ip_mat * a, ip_mat * b, 1);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h = b.h
 *      out.w = a.w + b.w
 *      out.k = a.k = b.k
 *
 * ip_mat_concat(ip_mat * a, ip_mat * b, 2);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h = b.h
 *      out.w = a.w = b.w
 *      out.k = a.k + b.k
 * */
ip_mat * ip_mat_concat(ip_mat * a, ip_mat * b, int dimensione){
    ip_mat *out;
    int x,y,z;

    switch (dimensione)
    {
    case 0:/*Asse x*/
        if(a -> w != b -> w || a -> k != b -> k){/*Se le dimensioni y e z non coincidono*/
            exit(1);
        }

        out = ip_mat_create(a -> h + b -> h, a -> w, a -> k,0.0);

        /* Debug
        printf("\ndimensioni a: h: %d w: %d k: %d\n",a -> h, a -> w, a -> k);
        printf("\ndimensioni b: h: %d w: %d k: %d\n",b -> h, b -> w, b -> k);
        printf("\nout creata con dimensioni h: %d w: %d z: %d\n",out -> h, out -> w, out -> k);
        */

        for(x = 0;x < a -> h;x++){
            for(y = 0;y < a -> w;y++){
                for(z = 0;z < a -> k;z++){
                    out -> data[x][y][z] = a -> data[x][y][z];/*Copio la prima ip_mat tale quale*/
                }
            }
        }

        for(x = 0;x < b -> h;x++){
            for(y = 0;y < b -> w;y++){
                for(z = 0;z < b -> k;z++){
                    out -> data[x + a -> h][y][z] = b -> data[x][y][z];/*Scalo sull'asse x della dimensione della seconda ip_mat*/ 
                }
            }
        }

        break;
    
    case 1:/*Asse y*/
        if(a -> h != b -> h || a -> k != b -> k){/*Se le dimensioni x e z non coincidono*/
            exit(1);
        }

        out = ip_mat_create(a -> h, a -> w + b -> w,a -> k,0.0);

        for(x = 0;x < a -> h;x++){
            for(y = 0;y < a -> w;y++){
                for(z = 0;z < a -> k;z++){
                    out -> data[x][y][z] = a -> data[x][y][z];/*Copio la prima ip_mat tale quale*/
                }
            }
        }

        for(x = 0;x < b -> h;x++){
            for(y = 0;y < b -> w;y++){
                for(z = 0;z < b -> k;z++){
                    out -> data[x][y + a -> w][z] = b -> data[x][y][z];/*Scalo sull'asse y della dimensione della seconda ip_mat*/ 
                }
            }
        }

        break;

    case 2:/*Asse z*/
        if(a -> h != b -> h || a -> w != b -> w){/*Se le dimensioni x e y non coincidono*/
            exit(1);
        }

        out = ip_mat_create(a -> h, a -> w, a -> k + b -> k,0.0);

        for(x = 0;x < a -> h;x++){
            for(y = 0;y < a -> w;y++){
                for(z = 0;z < a -> k;z++){
                    out -> data[x][y][z] = a -> data[x][y][z];/*Copio la prima ip_mat tale quale*/
                }
            }
        }

        for(x = 0;x < b -> h;x++){
            for(y = 0;y < b -> w;y++){
                for(z = 0;z < b -> k;z++){
                    out -> data[x][y][z + a -> k] = b -> data[x][y][z];/*Scalo sull'asse dei canali della dimensione della seconda ip_mat*/ 
                }
            }
        }

        break;

    default:/*Dimesione diversa da 0,1,2*/
        exit(1);
        break;
    }

    compute_stats(out);
    
    return out;
}
/*----------------------------------------------------------------------------------------*/
/**** PARTE 1: OPERAZIONI MATEMATICHE FRA IP_MAT ****/
/* Esegue la somma di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output. */
ip_mat * ip_mat_sum(ip_mat * a, ip_mat * b){
    ip_mat *out;
    int x,y,z;

    if(a -> h != b -> h || a -> w != b -> w || a -> k != b -> k){/*Controllo che tutte le dimensioni siano identiche*/
        exit(1);
    }

    out = ip_mat_create(a -> h, a -> w, a -> k,0.0);/*Istanzio out con le dimensioni di a*/

    for(x = 0;x < a -> h;x++){
        for(y = 0;y < a -> w;x++){
            for(z = 0;z < a -> k;z++){
                set_val(out,x,y,z,get_val(a,x,y,z)+get_val(b,x,y,z));
            }
        }
    }

    compute_stats(out);

    return out;
}
/*----------------------------------------------------------------------------------------*/
/* Esegue la sottrazione di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output.
 * */
ip_mat * ip_mat_sub(ip_mat * a, ip_mat * b){
    ip_mat *out;
    int x,y,z;

    if(a -> h != b -> h || a -> w != b -> w || a -> k != b -> k){/*Controllo che tutte le dimensioni siano identiche*/
        exit(1);
    }

    out = ip_mat_create(a -> h, a -> w, a -> k,0.0);/*Istanzio out con le dimensioni di a*/

    for(x = 0;x < a -> h;x++){
        for(y = 0;y < a -> w;x++){
            for(z = 0;z < a -> k;z++){
                set_val(out,x,y,z,get_val(a,x,y,z)-get_val(b,x,y,z));
            }
        }
    }

    compute_stats(out);

    return out;
}
/*----------------------------------------------------------------------------------------*/
/* Moltiplica un ip_mat per uno scalare c. Si moltiplica c per tutti gli elementi di "a"
 * e si salva il risultato in un nuovo tensore in output. */
ip_mat * ip_mat_mul_scalar(ip_mat *a, float c){
    ip_mat *out;
    int x,y,z;

    out = ip_mat_create(a -> h, a -> w, a -> k, 0.0);/*Istanzio out*/

    for(x = 0;x < a -> h;x++){
        for(y = 0;y < a -> w;y++){
            for(z = 0;z < a -> k;z++){
                set_val(out,x,y,z,get_val(a,x,y,z) * c);
            }
        }
    }

    compute_stats(out);

    return out;
}
/*----------------------------------------------------------------------------------------*/
/* Aggiunge ad un ip_mat uno scalare c e lo restituisce in un nuovo tensore in output. */
ip_mat *  ip_mat_add_scalar(ip_mat *a, float c){
    ip_mat *out;
    int x,y,z;

    out = ip_mat_create(a -> h, a -> w, a -> k, 0.0);/*Istanzio out*/

    for(x = 0;x < a -> h;x++){
        for(y = 0;y < a -> w;y++){
            for(z = 0;z < a -> k;z++){
                set_val(out,x,y,z,get_val(a,x,y,z) + c);
            }
        }
    }

    compute_stats(out);

    return out;
}
/*----------------------------------------------------------------------------------------*/
/* Calcola la media di due ip_mat a e b e la restituisce in output.*/
ip_mat * ip_mat_mean(ip_mat * a, ip_mat * b){
    ip_mat *out;
    int x,y,z;

    if(a -> h != b -> h || a -> w != b -> w || a -> k != b -> k){/*Le matrici devono avere dimensione UGUALE*/
        exit(1);
    }

    out = ip_mat_create(a -> h, a -> w, a -> k, 0.0);/*Istanzio out*/

    for(x = 0;x < a -> h;x++){
        for(y = 0;y < a -> w;y++){
            for(z = 0;z < a -> k;z++){
                set_val(out,x,y,z,(get_val(a,x,y,z) + get_val(b,x,y,z))/2.0);/*Calcolo la media*/
            }
        }
    }

    compute_stats(out);

    return out;
}
/*----------------------------------------------------------------------------------------*/