#include <gestion_temps.h>
#include <gestion_processus.h>

extern struct mem_clk horloge;

/* Fonctions de gestion du temps et interruptions */

/* Fonction affichant le temps (8 caractères : HH:MM:SS)
 * en haut à droite de l'écran,
 * prend en paramètre la chaine de caractère à afficher.
 */
void affiche_haut_droite(char *chaine, int ligne){
    int i, l = strlen(chaine);
    for(i=0 ; i<l ; i++){
        ecrit_car(ligne, (80-l) + i, *chaine);
        chaine ++;
    }
}

/* Traitement de l'interruption 32
 * Affiche le temps écoulé depuis le démarage du système
 * Appellé par la fonction assembleur traitant_IT_32
 * Acquittement de l'interruption
 */
void tic_PIT(void){
    char chaine[25];
    outb(0x20, 0x20);    // Acquittement de l'IT
    horloge.cmpt ++;
    if (horloge.cmpt == 50) { horloge.cmpt = 0; horloge.ss ++;
                              ordonnance();} // Ordonnancement des processus
    if (horloge.ss == 60)   { horloge.ss = 0;   horloge.mm ++; }
    if (horloge.mm == 60)   { horloge.mm = 0;   horloge.hh ++; }
    if (horloge.hh == 24)   { horloge.hh = 0;   printf("Un jour entier O_O\n"); }
    sprintf(chaine, "              %02d:%02d:%02d", horloge.hh, horloge.mm, horloge.ss);
    affiche_haut_droite(chaine, 0);
    affiche_etat_processus();
}

/* Initialisation de l'entrée 32 dans la tables des vecteurs d'interruption
 */
void init_traitant_IT(int32_t num_IT, void (*traitant)(void)){
    // Adresse de la table : 0x1000
    // Chaque Vecteur est composé de 2 mots de 4 octets chacun
    // 1er mot : 31..16 : KERNEL_CS, 15..0 : bits de poids faibles de l'adresse du traitant
    // 2em mot : 31..16 : bits de poids forts de l'adresse du traitant, 15..0 : constante 0x8E00
    uint32_t adr_fct = (uint32_t)(traitant);
    uint32_t *p = (uint32_t*)(0x1000 + (num_IT*8));
    // Ecriture des bits de poids faibles et de la constante KERNEL_CS
    *p = (KERNEL_CS << 16) | (adr_fct & 0xFFFF);
    p ++;
    // Ecriture des bits de poids forts et de la constante
    *p = (adr_fct & 0xFFFF0000) | 0x8E00;
}

/* Démasquage de l'IRQ0 pour autoriser les signaux en provenance de l'horloge
 * Fonction permettant de changer le bit voulu dans la table de masquage
 */
void masque_IRQ(uint32_t num_IRQ, bool masque){
    uint8_t tab_masque = inb(0x21);
    if (masque) {   // Forcer à 1
        tab_masque |= 1 << num_IRQ;
    }
    else {          // Forcer à 0
        tab_masque &= ~(1 << num_IRQ);
    }
    outb(tab_masque, 0x21);
}
