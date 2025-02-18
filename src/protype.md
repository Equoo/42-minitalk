

# Shared
- void SigWriteNbr(int pid, int bits, int n)
	- write number of 1 in 3 bits for 8 bits or 5 bits for 32 bits
	- Write n in bits to pid
	- SI Pid client deja envoyé, on sleep 1s
		- Si pas de message recu avant 0.5s
			- Renvoie
			- sleep 0.5s
			- si toujours rien
				- error
	----
- int ReadNbr(int bits)
	- Read n in bits
	- si apres validation server le client n'a pas envoyé de message avant 0.5 s
		- Renvoie
		- sleep 0.5s
		- si toujours rien
			- error

# Server
- Init receiver

## Receiver
- ReadNbr 32 de sont PID
- ReadNbr 32 de taille message
- Each char ReadNbr 8 > message
- Si total bits recu != taille message (32 + 32 + 8 * taille message)
	- error
- Affiche message

# Client
- Init server ping receiver
- 
- SigWriteNbr 32 de sont PID
- SigWriteNbr 32 de taille message
- Each char SigWriteNbr 8
