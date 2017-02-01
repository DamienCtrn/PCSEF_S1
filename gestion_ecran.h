#ifndef __GESTION_ECRAN_H__
#define __GESTION_ECRAN_H__

#include <cpu.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

// Couleur de Fond
#define F_NOIR 0x0000
#define F_BLEU 0x1000
#define F_BLANC 0xF000
#define F_GRIS 0xB8000
#define F_VERT 0x2000
#define F_CYAN 0xB000

// Couleur de Texte
#define T_NOIR 0x0000
#define T_CYAN 0x0B00
#define T_BLANC 0x0F00
#define T_ROUGE 0x0C00
#define T_MAGENTA 0xD00

struct param_ecran {
    uint32_t lig;
    uint32_t col;
};

/* Renvoie un pointeur sur la case mémoire
 * correspondant aux coordonnées fournies
 */
uint16_t *ptr_mem(uint32_t lig, uint32_t col);

/* Ecrit le caractère c aux coordonnées spécifiées
 */
void ecrit_car(uint32_t lig, uint32_t col, char c);

/* Parcours les lignes et les colonnes de l'écran
 * pour écrire dans chaque case un espace en blanc sur fond noir
 * (afin d'initialiser les formats dans la mémoire)
 */
void efface_ecran(void);

/* Place le curseur à la position donnée
 */
void place_curseur(uint32_t lig, uint32_t col);

/* Traite un caractère donné
 * (c'est à dire qui l'affiche si c'est un caractère normal
 * ou qui implante l'effet voulu si c'est un caractère de contrôle)
 */
void traite_car(char c);

/* Fait remonter d'une ligne l'affichage à l'écran
 */
void defilement(void);

/* Affiche une chaine de caractères à la position courante du curseur
 */
void console_putbytes(char *chaine, int32_t taille);

#endif
