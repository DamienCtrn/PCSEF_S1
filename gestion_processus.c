#include <gestion_processus.h>
#include <gestion_temps.h>


Process tab_p[TAILLE_TABLE_PROCESS];
Process *actif;
extern struct mem_clk horloge;

// void idle(void){
//     for (int i=0 ; i<3 ; i++){
//         printf("[idle] je tente de passer la main a proc1...\n");
//         printf("\n");
//         (tab_p[0]).state = Elu;
//         (tab_p[1]).state = Activable;
//         uint32_t *ancien = tab_p[0].reg_save;
//         uint32_t *nouveau = tab_p[1].reg_save;
//         ctx_sw(ancien, nouveau);
//         printf("[idle] il m'a redonne la main --'\n");
//     }
//     printf("[idle] bon je bloque tout, j'en ai marre\n");
//     hlt();
// }
//
// void proc1(void){
//     for (int j=0 ; j<3 ; j++){
//         printf("[proc1] idle m'a donne la main\n");
//         printf("[proc1] j'en veut pas de sa main moi\n");
//         printf("\n");
//         (tab_p[0]).state = Activable;
//         (tab_p[1]).state = Elu;
//         uint32_t *ancien = tab_p[1].reg_save;
//         uint32_t *nouveau = tab_p[0].reg_save;
//         ctx_sw(ancien, nouveau);
//     }
// }

void idle(void){
    int nb_proc = 3;
    while(1){
        //printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        // Creation dynamique de processus par idle
        for(int pid = 1 ; pid < TAILLE_TABLE_PROCESS ; pid++){
            if (tab_p[pid].state == Mort){
                char chaine[10];
                sprintf(chaine, "proc%02d", nb_proc+1);
                cree_processus(proc3, &pid, chaine);
                nb_proc ++;
            }
        }
        sti();
        hlt();
        cli();
    }
}

void proc1(void){
    for (int32_t i = 0; i < 4; i++) {
        printf("[temps = %u][%s] pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(2);
    }
    fin_processus();
}

void proc2(void){
    for (int32_t i = 0; i < 3; i++) {
        printf("[temps = %u][%s] pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(3);
    }
    fin_processus();
}

void proc3(void){
    for (int32_t i = 0; i < 2; i++) {
        printf("[temps = %u][%s] pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(4);
    }
    fin_processus();
}

int mon_pid(void){
    return actif->pid;
}

char *mon_nom(void){
    return actif->nom;
}

void ordonnance(void){
    // Récuperation des pid des processus
    // Ordonnancement par Round-Robin et Endormissement
    int old = mon_pid();
    int new = (old + 1) % TAILLE_TABLE_PROCESS;

    // Mise à jour de l'Etat des processus Endormis en fonction de l'heure
    for(int i = 1; i<TAILLE_TABLE_PROCESS; i++){
        if(tab_p[i].reveil < nbr_secondes() && tab_p[i].state == Endormi){
            tab_p[i].state = Activable;
        }
    }

    // On prend le prochain processus Activable
    while (new != old && tab_p[new].state != Activable){
        new = (new + 1) % TAILLE_TABLE_PROCESS;
    }

    // Mise à jour des états
    if (tab_p[old].state == Elu){
        tab_p[old].state = Activable;
    }
    tab_p[new].state = Elu;

    // Actualisation du pointeur
    actif = &tab_p[new];

    // Changement du contexte
    ctx_sw(tab_p[old].reg_save,tab_p[new].reg_save);
}

void initialise(void){
    // Initialisation des structures de processus
    // Initialisation de idle
    (tab_p[0]).pid = 0;
    (tab_p[0]).state = Elu;
    (tab_p[0]).reveil = 0;
    sprintf((tab_p[0]).nom," idle ");
    actif = &tab_p[0];

    // Initialisation des proc
    int i = 1;
    cree_processus(proc1, &i, "proc01");
    i++;
    cree_processus(proc2, &i, "proc02");
    i++;
    cree_processus(proc3, &i, "proc03");
}

// Creer et Initialise un processus
int cree_processus(void (*code)(void), int *pid, char *nom){
    if (*pid >= TAILLE_TABLE_PROCESS) return -1;
    (tab_p[*pid]).pid = *pid;
    (tab_p[*pid]).state = Activable;
    (tab_p[*pid]).reveil = 0;
    (tab_p[*pid]).reg_save[1] = (uint32_t)&tab_p[*pid].pile[TAILLE_PILE_PROCESS-1];
    (tab_p[*pid]).pile[TAILLE_PILE_PROCESS-1] = (uint32_t)code;
    sprintf((tab_p[*pid]).nom,nom);
    return *pid;
}

uint32_t nbr_secondes(void){
    return horloge.ss + (horloge.mm + horloge.hh*60)*60;
}

void dors(uint32_t nbr_secs){
    int pid = mon_pid();
    tab_p[pid].reveil = nbr_secondes() + nbr_secs;
    tab_p[pid].state = Endormi;
    ordonnance();
}

void affiche_etat_processus(void){
    int i;
    char chaine[25];
    for (i=0; i<TAILLE_TABLE_PROCESS; i++){
        sprintf(chaine,"[(%s) : %s]", tab_p[i].nom, state_to_string(tab_p[i].state));
        affiche_haut_droite(chaine, i+1);
    }
}

void fin_processus(void){
    actif->state = Mort;
    ordonnance();
}
