#include <cpu.h>
#include <inttypes.h>
#include <string.h>
#include <gestion_ecran.h>
#include <gestion_temps.h>
#include <gestion_processus.h>

// Variable globale dans gestion_ecran
extern struct param_ecran cursor;
// Variable globale dans gestion_temps
struct mem_clk horloge;
// Variables globales dans gestion_processus
extern Process tab_p[TAILLE_TABLE_PROCESS];
extern Process *actif;

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void)
{
    /* Interruptions et affichage du temps */
    // Initialisation de la table des vecteurs d'interruptions
    init_traitant_IT(32, traitant_IT_32);

    // Réglage et Initialisation de l'horloge pour une fréquence de 50Hz
    outb(0x34, 0x43);
    outb((uint8_t)( 0x1234DD / 50), 0x40);
    outb((uint8_t)((0x1234DD / 50) >> 8), 0x40);
    horloge.hh = 0; horloge.mm = 0; horloge.ss = 0; horloge.cmpt = 0;

    // Démasquage de l'IRQ0 (0 = autorisée)
    masque_IRQ(32, false);

    // Démasquage des interruptions externes
    // sti(); // On le fait dans les processus maintenant

    /* Affichage et gestion de l'écran */
    // Initialisation de l'écran
    efface_ecran();
    cursor.lig = 0; cursor.col = 0;
    place_curseur(cursor.lig, cursor.col);
    printf("HELLO\n");
    printf("\n");

    /* Gestion processus */
    initialise();

    /* Demarrage du processus par default */
    idle();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
