// Copyright (C) 2021, RTE (http://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Cas lineaire pur: on regarde si une variable n'intervient 
             pas dans les contraintes. Dans ce cas, on la fixe sur une 
             borne en fonctions du signe de son cout lineaire. 
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "prs_sys.h"

# include "prs_fonctions.h"
# include "prs_define.h"

# include "pne_fonctions.h"
# include "pne_define.h"

/*----------------------------------------------------------------------------*/

void PRS_VariablesHorsContraintes( PRESOLVE * Presolve, int * NbModifications )
{
int Cnt; int il; int Var; char LaVariableEstContrainte; int Nbs; int NombreDeVariables;
double * ValeurDeXPourPresolve; int * TypeDeBornePourPresolve;  int * Cdeb; int * Csui;
int * NumContrainte; double * A; char * ContrainteInactive; double * CoutLineaire;
double * BorneInfPourPresolve; double * BorneSupPourPresolve; PROBLEME_PNE * Pne;
int * TypeDeBorneNative;

Pne = (PROBLEME_PNE *) Presolve->ProblemePneDuPresolve;

NombreDeVariables = Pne->NombreDeVariablesTrav;
CoutLineaire = Pne->LTrav;
TypeDeBorneNative = Pne->TypeDeVariableTrav;

Cdeb = Pne->CdebTrav;
Csui = Pne->CsuiTrav;
NumContrainte = Pne->NumContrainteTrav;
A = Pne->ATrav;

ValeurDeXPourPresolve = Presolve->ValeurDeXPourPresolve;
BorneInfPourPresolve = Presolve->BorneInfPourPresolve;
BorneSupPourPresolve = Presolve->BorneSupPourPresolve;
TypeDeBornePourPresolve = Presolve->TypeDeBornePourPresolve;
ContrainteInactive = Presolve->ContrainteInactive;

Nbs = 0;

for ( Var = 0 ; Var < NombreDeVariables ; Var++ ) {
  if ( TypeDeBornePourPresolve[Var] == VARIABLE_FIXE ) continue;
  LaVariableEstContrainte = NON_PNE;
  il  = Cdeb[Var];
  while ( il >= 0 ) {
    Cnt = NumContrainte[il]; 
    if ( A[il] != 0.0 ) { 
      if ( ContrainteInactive[Cnt] == NON_PNE ) { 
        /* Dans ce cas la variable est contrainte et on ne fait rien */
        LaVariableEstContrainte = OUI_PNE;
        break;
      }
    }
    il = Csui[il]; 
  }

  if ( LaVariableEstContrainte == OUI_PNE ) continue;
	
  /* La variable n'est contrainte que par ses bornes */
  Nbs++;
  if ( CoutLineaire[Var] > 0. ) {   
    if ( TypeDeBornePourPresolve[Var] == VARIABLE_NON_BORNEE || 
	       TypeDeBornePourPresolve[Var] == VARIABLE_BORNEE_SUPERIEUREMENT ) {
      *NbModifications = 0;
      Pne->YaUneSolution = PROBLEME_NON_BORNE;
      printf("*** Phase de Presolve. Pas de solution car la variable %d est hors contraintes, son cout est %lf et elle est de type ",
              Var,CoutLineaire[Var]);
      return;
    }
    else ValeurDeXPourPresolve[Var] = BorneInfPourPresolve[Var];
  }
  else if ( CoutLineaire[Var] < 0. ) {
    if ( TypeDeBornePourPresolve[Var] == VARIABLE_NON_BORNEE || 
	       TypeDeBornePourPresolve[Var] == VARIABLE_BORNEE_INFERIEUREMENT ) {
      *NbModifications = 0;
      Pne->YaUneSolution = PROBLEME_NON_BORNE;
      printf("*** Phase de Presolve. Pas de solution car la variable %d est hors contraintes, son cout est %lf et elle est de type ",
              Var,CoutLineaire[Var]);
      return;
    }
    else ValeurDeXPourPresolve[Var] = BorneSupPourPresolve[Var]; 
  }
  else {
	  /* Le cout de la variable est nul, on la fixe a 0 (ou sur une borne si 0 est hors bornes) */
	  if (BorneSupPourPresolve[Var] > 0) {
		  BorneSupPourPresolve[Var] = 0;
	  }  
	  if (BorneInfPourPresolve[Var] > BorneSupPourPresolve[Var]) {
		  BorneSupPourPresolve[Var] = BorneInfPourPresolve[Var]; // pas necessaire mais �vite d�avoir BorneSup < BorneInf
	  }
	  ValeurDeXPourPresolve[Var] = BorneSupPourPresolve[Var];
  }
  TypeDeBornePourPresolve[Var] = VARIABLE_FIXE;

  TypeDeBorneNative[Var] = REEL; /* On change le type pour etre tranquille */
}

#if VERBOSE_PRS
  printf("-> Nombre de variables supprimees car n'intervenant pas dans les contraintes: %d\n",Nbs); 
#endif

*NbModifications = Nbs;

return;
}


