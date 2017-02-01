#include <gestion_ecran.h>


struct param_ecran cursor;

/* Fonctions de gestion de l'écran */

/* Renvoie un pointeur sur la case mémoire
 * correspondant aux coordonnées fournies
 */
uint16_t *ptr_mem(uint32_t lig, uint32_t col){
    return (uint16_t*)(0xB8000 + 2*(lig*80 + col));
}

/* Ecrit le caractère c aux coordonnées spécifiées
 */
void ecrit_car(uint32_t lig, uint32_t col, char c){
    uint16_t* p = ptr_mem(lig, col);
    uint16_t out = 0;
    out |= c;
    out |= F_BLANC | T_MAGENTA; // Couleur à changer ici
    *p = out;
}

/* Parcours les lignes et les colonnes de l'écran
 * pour écrire dans chaque case un espace en blanc sur fond noir
 * (afin d'initialiser les formats dans la mémoire)
 */
void efface_ecran(void){
    int i,j;
    for (i=0 ; i<25 ; i++){
        for (j=0 ; j<80 ; j++){
            ecrit_car(i,j,' ');
        }
    }
}

/* Place le curseur à la position donnée
 */
void place_curseur(uint32_t lig, uint32_t col){
    uint16_t pos = 80*lig + col;
    outb(0x0F,0x3D4);
    outb((uint8_t)(pos), 0x3D5);
    outb(0x0E,0x3D4);
    outb((uint8_t)(pos >> 8), 0x3D5);
}

/* Traite un caractère donné
 * (c'est à dire qui l'affiche si c'est un caractère normal
 * ou qui implante l'effet voulu si c'est un caractère de contrôle)
 */
void traite_car(char c){
    if (c > 31 && c < 127){
        ecrit_car(cursor.lig, cursor.col, c);
        if (cursor.col == 79){
            if (cursor.lig < 24){
                cursor.col = 0;
                cursor.lig ++;
            }
        }
        else{
            cursor.col ++;
        }
        place_curseur(cursor.lig, cursor.col);
    }
    else {
        switch (c) {
            case 8:     // Back
                if (cursor.col != 0){
                    cursor.col --;
                    place_curseur(cursor.lig, cursor.col);
                }
                break;
            case 9:     // Tabulation
                if (cursor.col < 79){
                    cursor.col += 8;
                    cursor.col -= cursor.col % 8;
                    place_curseur(cursor.lig, cursor.col);
                }
                break;
            case 10:    // Entrer
                if (cursor.lig < 24){
                    cursor.lig ++;
                    cursor.col = 0;
                    place_curseur(cursor.lig, cursor.col);
                }
                else{
                    defilement();
                    cursor.lig ++;
                    cursor.col = 0;
                    place_curseur(cursor.lig, cursor.col);
                }
                break;
            case 12:    // Reset Screen
                efface_ecran();
                cursor.lig = 0;
                cursor.col = 0;
                place_curseur(cursor.lig, cursor.col);
                break;
            case 13:    // Retour debut ligne
                cursor.col = 0;
                place_curseur(cursor.lig, cursor.col);
                break;
        }
    }
}

/* Fait remonter d'une ligne l'affichage à l'écran
 */
void defilement(void){
    int i;
    memmove(ptr_mem(0,0), ptr_mem(1,0), 3840);
    for (i=0 ; i<80 ; i++){ ecrit_car(24, i, ' '); }
    cursor.lig --;
    place_curseur(cursor.lig, cursor.col);
}

/* Affiche une chaine de caractères à la position courante du curseur
 */
void console_putbytes(char *chaine, int32_t taille){
    int i;
    for (i=0 ; i<taille ; i++){
        traite_car(*chaine);
        chaine ++;
    }
}
