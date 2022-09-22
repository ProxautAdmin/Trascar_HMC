#ifndef _EVENTI_H
#define _EVENTI_H

#define EV_NUOVAMISSIONE	'M'	// "M,id,sor,hsor,des,hdes,pri,tipo,miss_prec"
#define EV_INITAGV			'I'	// "I,agv,pos"
#define EV_SPOSTAMENTOAGV	'P'	// "P,agv,pos"
#define EV_PRELIEVOAGV		'^'	// "^,agv,pos"
#define EV_DEPOSITOAGV		'D'	// "D,agv,pos"
#define EV_DROPAGVLOAD		'°'	// "°,agv,pos" // agv effettua deposito ma rimane carico (MULTIPICK)
#define EV_INCARICAAGV		'G'	// "G,agv,pos"
#define EV_STATOAGV			'T'	// "T,agv,stato"
#define EV_MANUALBC			'R'	// "R,id_agv , carrello con livello batteria bassa"
#define EV_WAITPOSITION		'V'	// "V,id_agv,1 o 0 ,Nodo, abilita-disabilita lo spostamento di un AGV in pos di attesa(Nodo).Se Nodo=0 trova cb manuale + vicino"
#define EV_COMANDOAGV		'C'	// "C,agv,pos"
#define EV_STATOMISSIONE	'S'	// "S,id,stato"
#define EV_ALLARMIAGV		'L'	// "L,id,allarmi"
#define EV_AGGIUNGICB		'A'	// "A,cb_number"
#define EV_CANCELLACB		'E'	// "E,cb_number"
#define EV_MISSPOLICY		'B'	// "B,0=miss/1=cb"
#define EV_FMESSAGE 		'X'	// "X,id,Registro8" invia tramite Fmessage il registro 8 all'AgvComm
#define EV_BCBACK			'F'	// "F,cb_number"
#define EV_WAITBC			'N'	// "N,id,cb_number"
#define EV_ATTESA			'W'	// "W,R,id,sorg,dest,string" o "W,N,id,nodo,op" (op = 'P'rel/'D'ep)
#define EV_CONSENSO			'O'	// "O,R,id,sorg,dest,st" o "O,N,id,nodo,op" (op = 'P'rel/'D'ep)
#define EV_BIT				'H'	// "H,id,sorg,dest,stringa" alzo bit a supervisore (op = 'P'rel/'D'ep)
#define ST_PRELIEVOESEGUITO	'P'	// "M,idmiss"
#define ST_MISSCOMPLETATA	'C'	// "C,idmiss"
#define ST_MISSABORTITA 	'A'	// "A,idmiss"
#define EV_CHANGEPRIOR		'Q'	// "Q,Idmiss,priority" modifica la priorità di una missione non ancora assegnata
#define EV_CHANGEIDAGV		'U'	// "U,IdAgvold,IdAgvnew" modifica l'idagv di una missione in lista con l'idagv nuovo passato
#define EV_RELOADBC 		'Z'	// "Z" ricarica i carica batterie e reimposta il carica batterie statico ad ogni carrello se presente
#define EV_ESCLUDIAGV 		'Y'	// "Y" esclude include un agv, Y,nagv,Escludi : 1 = escluso, 0 = incluso nel polling dell'agvcomm
#define EV_DISABILITAAGV	'!'	// "!" forza l'agv in posizione 0 con abort e agv scarico
#define EV_SEMIAUTO			'$'	// "$,ID,POS,VAIALNODO"
#define EV_BATTERIA			'!'	// "!,ID,livello batteria
#define EV_CHARGE_ON		'='	// "!,ID,stato     segno uscita carica batterie agv
#define EV_STATO_AGV		'?'	// "?,ID,stato     segno agv in stand by e bumper bypass premuti
#define EV_INVIOLOAD		'L'	// "L,id,loaded" invio stato carico loaded a supervisione
#define EV_FINELOWBATT		'/'	// "/,id" resetto bit low battery sullo status dell'agv dopo essere stato in carica per il tempo
#define EV_STATOVAR 		'*'	// "*,id,nomevar,valore" passo lo stato di una variabile all'agvx
#define EV_STATO_VAR 		'-'	// "-,id,nomevar,valore" passo lo stato di una variabile dall'agvx alla supervisione
#define EV_READVAR   		'@'	// "@,id,nomevar,tipovariabile" richiesta da parte di supervisione dello stato di una variabile all'agvx
#define EV_QUOTAASSOLUTA	'('	// "+,id,x,y" passo le quote x e y dall'agvcomm all'agvx
#define EV_QUOTA_ASSOLUTA	'+'	// "(,id,x,y" passo le quote x e y dall'agvx al supervisore

typedef char Evento[1024];
 
#endif

