// Copyright (C) 2021, RTE (http://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Stockage des coupes generees en un noeud, ce module est 
             appele par le calcul des coupes a chaque fois qu'une coupe
             est disponible 
        
   AUTEUR: R. GONZALEZ

************************************************************************/

#include "bb_sys.h"
#include "bb_define.h"
#include "bb_fonctions.h"

# ifdef BB_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
  # include "bb_memoire.h"
# endif

/*---------------------------------------------------------------------------------------------------------*/

void BB_StockerUneCoupeGenereeAuNoeud( BB * Bb, int NombreDeTermes, double * Coefficient, int * IndiceDeLaVariable,
                                       double SecondMembre, char TypeDeCoupe, char PresenceDeVariableEntiereDansLaCoupe )
{
int i; int IndiceDeCoupe;  NOEUD * NoeudCourant;
/*
printf("Partie BRANCH AND BOUND, STOCKAGE de la coupe  \n");
*/
NoeudCourant  = Bb->NoeudEnExamen;
   
/* On Stocke toutes les coupes au noeud racine */
NoeudCourant  = Bb->NoeudRacine;

IndiceDeCoupe = NoeudCourant->NombreDeCoupesGenereesAuNoeud;

  /* Premiere allocation de structure */
if ( IndiceDeCoupe == 0 ) {
  NoeudCourant->CoupesGenereesAuNoeud = (COUPE **) malloc( ( IndiceDeCoupe + 1 ) * sizeof( void * ) );
  NoeudCourant->NumeroDesCoupeAjouteeAuProblemeCourant = (int *) malloc( ( IndiceDeCoupe + 1 ) * sizeof( int ) );
}
else {
  NoeudCourant->CoupesGenereesAuNoeud = (COUPE **) realloc( NoeudCourant->CoupesGenereesAuNoeud , 
                                                            ( IndiceDeCoupe + 1 ) * sizeof( void * ) );
  NoeudCourant->NumeroDesCoupeAjouteeAuProblemeCourant = (int *) realloc( NoeudCourant->NumeroDesCoupeAjouteeAuProblemeCourant , 
                                                                           ( IndiceDeCoupe + 1 ) * sizeof( int ) );
}
if ( NoeudCourant->CoupesGenereesAuNoeud == NULL || NoeudCourant->NumeroDesCoupeAjouteeAuProblemeCourant == NULL ) {
  printf("\n Saturation memoire dans la partie branch & bound, fonction: BB_StockerUneCoupeGenereeAuNoeud 1\n");
  Bb->AnomalieDetectee = OUI;
  longjmp( Bb->EnvBB , Bb->AnomalieDetectee );  
}

/* Stockage de la coupe */
NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe] = (COUPE *) malloc( sizeof( COUPE ) );	  
if ( NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe] == NULL ) {
  printf("\n Saturation memoire dans la partie branch & bound, fonction: BB_StockerUneCoupeGenereeAuNoeud 2\n"); 
  Bb->AnomalieDetectee = OUI;
  longjmp( Bb->EnvBB , Bb->AnomalieDetectee ); 
}

/*NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->VariableCause = VariableCause;*/
NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->TypeDeCoupe   = TypeDeCoupe;
if ( Bb->NoeudEnExamen == Bb->NoeudRacine ) {
  NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->CoupeRacine = OUI;
}
else {
  NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->CoupeRacine = NON;
}
NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->NombreDeTermes = NombreDeTermes;
NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->SecondMembre = SecondMembre;
NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->PresenceDeVariableEntiereDansLaCoupe = PresenceDeVariableEntiereDansLaCoupe;

NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->Coefficient        = (double *) malloc( NombreDeTermes * sizeof( double ) );
NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->IndiceDeLaVariable = (int *)   malloc( NombreDeTermes * sizeof( int ) );
if ( NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->Coefficient        == NULL || 
     NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->IndiceDeLaVariable == NULL) {
  printf("\n Saturation memoire dans la partie branch & bound, fonction: BB_StockerUneCoupeGenereeAuNoeud\n"); 
  Bb->AnomalieDetectee = OUI;
  longjmp( Bb->EnvBB , Bb->AnomalieDetectee ); 
}

for ( i = 0 ; i < NombreDeTermes ; i++ ) {
  NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->Coefficient[i]        = Coefficient[i];
  NoeudCourant->CoupesGenereesAuNoeud[IndiceDeCoupe]->IndiceDeLaVariable[i] = IndiceDeLaVariable[i];
}

NoeudCourant->NumeroDesCoupeAjouteeAuProblemeCourant[Bb->NombreDeCoupesAjoutees] = NoeudCourant->NombreDeCoupesGenereesAuNoeud;

/* Mise a jour du nombre de coupes */
NoeudCourant->NombreDeCoupesGenereesAuNoeud++;
if ( TypeDeCoupe == 'G' ) NoeudCourant->NombreDeG++;
else if ( TypeDeCoupe == 'K' ) NoeudCourant->NombreDeK++;
else if ( TypeDeCoupe == 'I' ) NoeudCourant->NombreDeI++;

Bb->NombreDeCoupesAjoutees++;

return; 
}



