// Copyright (C) 2021, RTE (http://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0


/***********************************************************************************

   FONCTION: 

                   
   AUTEUR: R. GONZALEZ

************************************************************************************/

# include "pi_sys.h"

# include "pi_fonctions.h"
# include "pi_define.h"

/*------------------------------------------------------------------------*/
/*                          Initialisation                                */ 

void PI_SplitContraintes( PROBLEME_PI * Pi )
{
int i      ; int NbCntTot; int   NbTermes1; int NbTermes2; 
int SvNuvar; int OldNbTerm ; double SvA      ; int EmplacementLibreDansA;
int il     ;

int SeuilSplitContrainte; 

SeuilSplitContrainte = 100;

/* Attention on suppose que les contraintes sont contigues en entree */
EmplacementLibreDansA = Pi->Mdeb  [Pi->NombreDeContraintes-1] 
                      + Pi->NbTerm[Pi->NombreDeContraintes-1];

/* Matrice des contraintes */
NbCntTot = Pi->NombreDeContraintes;
for ( i = 0 ; i < Pi->NombreDeContraintes ; i++ ) {

  if( Pi->NbTerm[i] >= (2 * SeuilSplitContrainte) ) {

  /* On split la contrainte en 2 */
  NbTermes1 = (int) (0.5 * Pi->NbTerm[i]); 
  NbTermes2 = Pi->NbTerm[i] - NbTermes1;
  /* On cree une variable de couplage */
  Pi->NombreDeVariables++;
  /* dont on initialisera les bornes apres */
  Pi->Umin  [Pi->NombreDeVariables - 1] = -100;
  Pi->Umax  [Pi->NombreDeVariables - 1] =  100;
  Pi->Q     [Pi->NombreDeVariables - 1] =  0.;
  Pi->L     [Pi->NombreDeVariables - 1] =  0.;

  /* Remodelage de la contrainte i */
  SvNuvar   = Pi->Indcol [ Pi->Mdeb[i] + NbTermes1 ];
  SvA       = Pi->A     [ Pi->Mdeb[i] + NbTermes1 ];
  OldNbTerm = Pi->NbTerm[i];

  Pi->NbTerm[i]                               = NbTermes1 + 1; 
  Pi->Indcol[ Pi->Mdeb[i] + Pi->NbTerm[i] - 1 ] = Pi->NombreDeVariables - 1;
  Pi->A     [ Pi->Mdeb[i] + Pi->NbTerm[i] - 1 ] = 1.;

  /* On cree la contraintes de couplage */
  Pi->Mdeb  [NbCntTot] = EmplacementLibreDansA;
  Pi->NbTerm[NbCntTot] = NbTermes2 + 1; 
  Pi->B     [NbCntTot] = 0.;

  Pi->A    [EmplacementLibreDansA] = SvA;
  Pi->Indcol[EmplacementLibreDansA] = SvNuvar;
  EmplacementLibreDansA++;
  for ( il = Pi->Mdeb[i] + Pi->NbTerm[i] ;  il < Pi->Mdeb[i] + OldNbTerm ; il++ , EmplacementLibreDansA++) {
    Pi->A    [EmplacementLibreDansA] = Pi->A[il];
    Pi->Indcol[EmplacementLibreDansA] = Pi->Indcol[il]; 
  }
  Pi->A    [EmplacementLibreDansA] = -1;
  Pi->Indcol[EmplacementLibreDansA] = Pi->NombreDeVariables - 1;
  EmplacementLibreDansA++;

  NbCntTot++;

  }

}

Pi->NombreDeContraintes = NbCntTot;
printf(" Apres split NombreDeVariables %d NombreDeContraintes %d \n",Pi->NombreDeVariables,Pi->NombreDeContraintes);

return;
}	       

