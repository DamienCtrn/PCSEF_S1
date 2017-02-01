#ifndef __GESTION_TEMPS_H__
#define __GESTION_TEMPS_H__

#include <cpu.h>
#include <inttypes.h>
#include <gestion_ecran.h>
#include <stdio.h>
#include <stdbool.h>
#include <segment.h>

struct mem_clk {
    uint8_t hh;
    uint8_t mm;
    uint8_t ss;
    uint8_t cmpt;
};

/* Fonction affichant le temps (8 caractères)
 * en haut à droite de l'écran,
 * prend en paramètre la chaine de caractère à afficher.
 */
void affiche_haut_droite(char *chaine, int ligne);

/* Traitant de l'interruption en assembleur dans le fichier traitants.S
 */
void traitant_IT_32(void);

/* Traitement de l'interruption 32
 * Affiche le temps écoulé depuis le démarage du système
 * Appellé par la fonction assembleur traitant_IT_32
 * Acquittement de l'interruption
 */
void tic_PIT(void);

/* Initialisation de l'entrée 32 dans la tables des vecteurs d'interruption
 */
void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

/* Démasquage de l'IRQ0 pour autoriser les signaux en provenance de l'horloge
 * Fonction permettant de changer le bit voulu dans la table de masquage
 */
void masque_IRQ(uint32_t num_IRQ, bool masque);


#endif
