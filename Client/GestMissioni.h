// ---------------------------------------------------------------------------

#ifndef GestMissioniH
#define GestMissioniH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "db.h"

class TdmGestMissioni : public TDataModule
{
__published: // IDE-managed Components
        private : // User declarations
    public : // User declarations
        __fastcall TdmGestMissioni(TComponent* Owner);
    void InsertUDC1Click(TMenuItem *Item, int agvdaspostare);
    void DepositaUDC1Click(TMenuItem *Item);
    void PesaUDC1Click(TMenuItem *Item);
    void MoveMisMan1Click(TMenuItem *Item)     ;
    void DropAgv1Click(TMenuItem *Item, int agvdaspostare);
    void MoveAgv1Click(TMenuItem *Item, int agvdaspostare)     ;
	void GestisciMissioniPeso(int pos)  ;
	void MisManSpost1Click(TMenuItem *Item)  ;
    void MisManSpostPLCClick(TMenuItem *Item) ;

};

// ---------------------------------------------------------------------------
extern PACKAGE TdmGestMissioni *dmGestMissioni;
// ---------------------------------------------------------------------------
#endif
