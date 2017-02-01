#ifndef __GESTION_PROCESSUS_H__
#define __GESTION_PROCESSUS_H__

#include <stdio.h>
#include <cpu.h>
#include <inttypes.h>
#include <gestion_ecran.h>

#define TAILLE_NOM 15
#define TAILLE_PILE_PROCESS 512
#define TAILLE_TABLE_PROCESS 4

typedef enum State {Elu, Activable, Endormi, Mort} STATE;

static inline char* state_to_string(STATE s){
    static char *strings[] = {"      Elu", "Activable", "  Endormi", "     Mort"};
    return strings[s];
}

typedef struct process {
    int pid;
    char nom[TAILLE_NOM];
    STATE state;
    uint32_t reveil;
    uint32_t reg_save[5];
    uint32_t pile[TAILLE_PILE_PROCESS];
} Process;

void idle(void);

void proc1(void);

void proc2(void);

void proc3(void);

int mon_pid(void);

char *mon_nom(void);

void ordonnance(void);

void initialise(void);

// Creer et Initialise un processus avec
int cree_processus(void (*code)(void), int *pid, char *nom);

void dors(uint32_t nbr_secs);

void fin_processus(void);

void affiche_etat_processus(void);

uint32_t nbr_secondes(void);

void ctx_sw(uint32_t* old_reg_save, uint32_t* new_reg_save);

#endif
