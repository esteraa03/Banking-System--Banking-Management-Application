#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RON, EUR, GBP } Currency;
typedef enum { user, admin } UserType;

typedef struct account { // definire sublista
	struct account* urm; //  inlantuire
	Currency currency;
	float sold;
	char iban[20];// elemente lista
}account;

typedef struct User { // definire lista
	struct User* urm; // prima inlantuire
	struct account* headAccount; // radacina pentru conturi
	struct account* tailAccount; // ultima adresa a sublistei conturi
	char username[20], cnp[13], pass[20];// elemente lista
	UserType userType;
}User;

struct User* UserLogged = NULL;

const char* TypeUserToString(UserType userType)//functie de tip const char*, adica returneaza un pointer catre un ?ir de caractere constant
{
	switch (userType)
	{
	case user: return "user"; break;
	case admin: return "admin"; break;
	default: return "nedefinit";
		break;
	}
}

enum UserType stringToUserType(const char* userType)//Aceasta func?ie are tipul de întoarcere enum UserType
{
	if (strcmp("user", userType) == 0) return user;
	if (strcmp("admin", userType) == 0) return admin;
	return user;
}


const char* currencyToString(Currency currency) {
	switch (currency)
	{
	case RON: return "RON"; break;
	case EUR: return "EUR"; break;
	case GBP: return "GBP"; break;
	default: return "nedefinit";
		break;
	}
}
enum Currency stringToCurrency(const char* currency) {
	if (strcmp("RON", currency) == 0) return RON;
	if (strcmp("EUR", currency) == 0) return EUR;
	if (strcmp("GBP", currency) == 0) return GBP;
	return RON;
}

struct account* SearchAccount(struct account* head, const char *iban) {
	struct account* q = head;
	while (q) {
		if (strcmp(q->iban, iban) == 0) return q;
		q = q->urm;
	}
	return NULL;
}
struct User* AddToEndAccount(struct User* user, struct account* p) {
	if (user->headAccount == NULL) {
		user->headAccount = p;
		user->tailAccount = p;
	}
	else {
		user->tailAccount->urm = p;
		user->tailAccount = p;
	}
	return user;
}
void RemoveAllAccounts(struct account* head) {
	struct account* aux;
	while (head) {
		aux = head;
		head = head->urm;
		free(aux);
	}

}
void ShowAccounts(struct account* headAccount) {
	struct account* q = headAccount;
	printf("--------------------------------------\n");
	while (q) {
		printf("Iban = %s moneda =  %s\n", q->iban, currencyToString(q->currency));
		q = q->urm;
	}
	printf("--------------------------------------\n");
}

struct User* SearchUserByUsername(struct User* head, const char* username) {
	struct User* q = head;
	while (q) {
		if (strcmp(q->username, username) == 0) return q;
		q = q->urm;
	}
	return NULL;
}

struct User* SearchUserByCNP(struct User* head, const char* cnp) {
	struct User* q = head;
	while (q) {
		if (strcmp(q->cnp, cnp) == 0) return q;
		q = q->urm;
	}
	return NULL;
}
struct User* AddSortedUser(struct User* head, struct User* p) {// functia de adaugare sortata
	struct User* q = head;// User folosit pentru parcurgea listei
	if (head == NULL) // (!head) verificam daca sunt elemente in lista
	{
		head = p;// daca nu sunt capul listel devine noul element
	}
	else if (strcmp(head->username, p->username) > 0) { // verificam daca elementul adaugat trebuie adaugat in capul listei
		p->urm = head; //elementul aduaugat va avea in pareta dreapta lista existenta
		head = p; // noul element va devieni capatul(radidacian listi)
		return head;
	}
	else {
		while (q->urm != NULL && strcmp(q->urm->username, p->username) < 0) { // parcurgem lista ca sa gasim pozitia de inserare
			q = q->urm;// parcurgere lista
		}
		p->urm = q->urm;// elementu nou va avea in dreapta ce avea poztia la care s-a gasit inserarea
		q->urm = p;
	}
	return head;
}
struct User* RemoveUser(struct User* head, char username[]) { //sterge User
	struct User* aux = NULL, * q = NULL;// Userul aux este pentreu retinire tempara User sters, q este folosit pentru parcurgere
	if (head == NULL) {// verificam daca capul este null altfel return null ca si adresa
		return NULL;
	}
	else if (strcmp(head->username, username) == 0) {// verificam daca este capul listei Userul sters
		aux = head;// salvam temporar Userul
		head = head->urm;// capul devine umaatorul element
		RemoveAllAccounts(aux->headAccount);
		free(aux);// returnam adresa Userul pe care vrem sa-l stergem in caz ce se vrea prelucat ceva dupa
		return head;
	}
	else {
		q = head;// daca nu e la inceput parcurgem lista
		while (q->urm != NULL && strcmp(q->urm->username, username) != 0) {// ca la adaugarea sorata cautam pozita anterioara Userul pe care vrem sa-l stergem
			q = q->urm;// parcurgem lista
		}
		if (q->urm == NULL) {// daca s-a ajuns la sfarsitul listei si nu s-a gasit
			return;// returnam null deoarece Userul nu se afla in lista
		}
		else {
			aux = q->urm;// altfel salvam Userul urmator(Userul ce trebuie stres)
			q->urm = q->urm->urm;// Userul anterior va pointa catre Userul din dtreapa celui sters
			RemoveAllAccounts(aux->headAccount);
			free(aux);// returnam Userul pe care vrem sa-l stergem
		}
		return head;
	}
}
void ShowUsers(struct User* head) {// toat functiile le am implementat sa primeasca head deaorece e mai ok asa
	struct User* p = head;// ne punem pe capul listei

	while (p != NULL) {// cat timp nu s-a ajuns la sfarsitul fiserului
		printf("username = %s\nCNP = %s\n", p->username, p->cnp);// afisam datele despre utilziator
		ShowAccounts(p->headAccount);
		p = p->urm;// parcurgem lista
	}
}

void LoginUser(struct User* head) {
	char username[20], pass[20];
	struct User* user;
	printf("\nusername: "); scanf_s("%s", username, sizeof(username));
	printf("\npassword: "); scanf_s("%s", pass, sizeof(pass));
	user = SearchUserByUsername(head, username);
	if (user) {
		if (strcmp(user->pass, pass) == 0) {
			UserLogged = user;
		}
		else {
			printf("\nParola este gresita");
		}
	}
	else
	{
		printf("\nUtilizatorul nu a fost gasit");
	}
}

struct account* SelectedByCurrency( struct User* head, const char* type_account)
{
	struct account* current = head->headAccount;
	Currency selectedCurrency = stringToCurrency(type_account);

	while (current != NULL)
	{
		if (current->currency== selectedCurrency)
		{
			return current;
		}
		current = current->urm;
	}
	return NULL;
}

void MeniuUser(struct User* head) {
	int opt;
	int a=0, b=0, c=0, d=0;
	struct account* selected_account=NULL;
	do {
		printf("\n0. Dezautentificare");
		printf("\n1.Selectare cont pentru operatii (Iesire/Adaugare/Retragere/Transfer/Afisare extrase)");
		printf("\n2.Schimbare parola");
		printf("\nOptiunea aleasa: "); scanf_s("%d", &opt);
		switch (opt)
		{
		case 0:
			UserLogged = NULL;
			break;
		case 1:
			printf("\nCe cont doriti sa folositi pentru operatii?(RON/EUR/GBP)\n");
			ShowAccounts(UserLogged->headAccount);

			char type_account[10];
			scanf("%s", type_account);

			selected_account = SelectedByCurrency(UserLogged, type_account);

			if (selected_account == NULL)
			{
				printf("Nu s-a putut selecta contul\n");
				break;
			}
			int operatia;
			float add = 0.0;
			float sub = 0.0;
			float transfer = 0.0;
			char username[10];
			do
			{
				printf("\nAlegeti operatia: (0/1/2/3/4)");
				scanf("%d", &operatia);

				if (operatia == 1)
				{
					printf("\nCe suma doriti sa adaugati?");
					scanf("%f", &add);
					selected_account->sold += add;
					printf("\nAti adaugat %f de %s in contul dvs. ", add, type_account);
				}
				else if (operatia == 2)
				{
					printf("\nCe suma doriti sa extrageti?");
					scanf("%f", &sub);

					if (selected_account->sold >= sub && selected_account->sold > 0) {
						selected_account->sold -= sub;
						printf("\nAti extras %f de %s din contul dvs.", sub, type_account);
					}
					else
					{
						sub = 0.0;
						printf("\nFonduri insuficiente");
					}
				}
				else if (operatia == 3)
				{
					printf("\nIn ce cont doriti sa transferati?\nusername: ");
					scanf("%s", username);

					struct User* destinatar = SearchUserByUsername(head, username);
					if (destinatar == NULL) {
						printf("\nUtilizatorul nu a fost gasit");
						break;
					}
	
				    struct account* dest_account = SelectedByCurrency(destinatar, type_account);
						if (dest_account == NULL) {
							printf("\nContul destinatarului nu a fost gasit");
							break;
						}

				printf("\nCe suma doriti sa transferati?");
				scanf("%f", &transfer);
				if (selected_account->sold >= transfer) 
				{
				  selected_account->sold -= transfer;
				  dest_account->sold += transfer;
				  printf("\nAti transferat %f de %s in contul %s ", transfer, type_account, username); 
				}
				else { 
					transfer = 0.0;
					printf("\nFonduri insuficiente"); }
				}
				else if (operatia == 4)
				{
					printf("Afisare extrase: ");
				    printf("\nAti adaugat %f de %s in contul dvs. ", add, type_account);
					printf("\nAti extras %f de %s din contul dvs.", sub, type_account);
					printf("\nAti transferat %f de %s in contul %s ", transfer, type_account, username);
				}
			}while (operatia!=0);
			break;
		case 2: 
			printf("Parola noua: ");
			char buffer[20];
			scanf("%s", buffer);
			strcpy(UserLogged->pass, buffer);
			printf("Noua parola este: %s", UserLogged->pass);
			break;
		default:
			break;
		}
	} while (opt);
}

void AddUser(struct User* head) {
	struct User* newUser = (struct User*)malloc(sizeof(struct User));
	newUser->urm = NULL;
	newUser->headAccount = NULL;
	newUser->tailAccount = NULL;

	printf("\nusername: ");
	scanf("%s", newUser->username);

	printf("\npassword: ");
	scanf("%s", newUser->pass);

	printf("\ncnp: ");
	scanf("%s", newUser->cnp);

	newUser->userType = user;
	newUser->headAccount = NULL;
	newUser->tailAccount = NULL;
	head = AddSortedUser(head, newUser);
}

void AddAccount(struct User* head)
{
	struct account* newAccount = (struct account*)malloc(sizeof(struct account));
	if (!newAccount) {
		printf("Eroare la alocarea memoriei pentru contul nou.\n");
		return;
	}
	newAccount->urm = NULL;

	printf("\niban: ");
	scanf("%s", newAccount->iban);

	printf("\nMoneda: ");
	char moneda[5];
	scanf("%s", moneda);
	newAccount->currency = stringToCurrency(moneda);
	AddToEndAccount(head, newAccount);
	printf("Contul IBAN a fost adaugat cu succes.\n");
}

void RemoveOneAccount(struct account** head, const char* iban)
{
	struct account* aux = *head, * q = NULL;

	if (aux != NULL && strcmp(aux->iban, iban) == 0) {
		*head = aux->urm;
		free(aux);
		printf("Contul IBAN %s a fost sters\n", iban);
		return;
	}

	while (aux != NULL && strcmp(aux->iban, iban) != 0) {
		q = aux;
		aux = aux->urm;
	}

	if (aux == NULL) {
		printf("Contul IBAN %s nu a fost gasit\n", iban);
		return;
	}

	q->urm = aux->urm;
	free(aux);
	printf("Contul IBAN %s a fost sters.\n", iban);
}

void MeniuAdmin(struct User* head) {
	int opt;
	do {
		printf("\n0. Dezautentificare");
		printf("\n1.Adaugare utilizator");
		printf("\n2.Adaugare cont IBAN");
		printf("\n3.Afisare utilizatori si extrase");
		printf("\n4.Sterge IBAN");
		printf("\n5.Editare date user dupa CNP(nume, prenume)");
		printf("\n6.Schimbare parola");
		printf("\nOptiunea aleasa: "); scanf_s("%d", &opt);
		switch (opt)
		{
		case 0:
			UserLogged = NULL;
			break;
		case 1:
			AddUser(head);
			break;
		case 2:
			printf("\nAdaugare cont iban pentru utilizatorul: ");
			char nume[10];
			scanf("%s", nume);
			struct User* selectedUser = SearchUserByUsername(head, nume);
			AddAccount(selectedUser);
			break;
		case 3: 
			ShowUsers(head);
			break;
		case 4:
			printf("\nStergere cont iban pentru utilizatorul: ");
			char name[10];
			scanf("%s", name);
			struct User* selecteduser = SearchUserByUsername(head, name);

			printf("\nIntroduceti ibanul pe care doriti sa-l stergeti: ");
			char iban[10];
			scanf("%s", iban);
			//struct account* selectedAccount = SearchOneAccount(head, iban);
			RemoveOneAccount(&selecteduser->headAccount, iban);
			break;
		case 5: 
			printf("\nIntroduceti CNP-ul: ");
			char cnp[10];
			scanf("%s", cnp);
			struct User* alteredUser = SearchUserByCNP(head, cnp);

			if (SearchUserByCNP != 0) {
				printf("\nA fost gasit. Introduceti noile date. \n username: ");
				char newusername[10];
				scanf("%s", newusername);
				strcpy(alteredUser->username, newusername);
				printf("\nNoul username este: %s", alteredUser->username);
			}
			else printf("\nUtilizatorul nu a fost gasit");
			break;
		case 6:
			printf("\nParola noua: ");
			char buffer[20];
			scanf("%s", buffer);
			strcpy(UserLogged->pass, buffer);
			printf("\nNoua parola este: %s", UserLogged->pass);
			break;
		default:
			break;
		}
	} while (opt);
}


struct User* ReadUsersFromFile(const char* filePath) {
	struct User* p, * q = NULL;// Userul nou adaugat
	struct User* head = NULL;// capul listei de utilizatori
	FILE* f = NULL;
	fopen_s(&f, filePath, "r");
	if (f == NULL) {
		printf("\nFiserul de utilizatori nu a putut fi deschis");
		return NULL;
	}
	while (!feof(f))
	{
		p = (struct User*)malloc(sizeof(struct User));
		fscanf_s(f, "%s", p->username, sizeof(p->username)); // fscanf_s pentru siruri si caractere necesita si numarul de elemente maxim citit
		fscanf_s(f, "%s", p->cnp, sizeof(p->cnp)); // %*c se foloseste pentru a citi \n de la sfarst de rand si face discard
		fscanf(f, "%s", p->pass);
		fscanf(f, "%d%*c", &p->userType);
		p->urm = NULL;
		p->headAccount = NULL;
		p->tailAccount = NULL;
		q = SearchUserByCNP(head, p->cnp);
		if (q || feof(f) || SearchUserByUsername(head, p->username)) {
			free(p);
		}
		else {
			head = AddSortedUser(head, p);
		}
	}
	fclose(f);
	return head;
}
void ReadAccountsFromFile(struct User* head, const char* filePath) {
	struct User* q = NULL;
	
	struct account* aux = NULL, * temp = NULL;
	char cnp[13], currency[13];
	FILE* f = NULL;
	

	fopen_s(&f, filePath, "r");

	while (!feof(f))
	{
		aux = (struct account*)malloc(sizeof(struct account));
		fscanf_s(f, "%s", cnp, sizeof(cnp));// fscanf_s pentru siruri si caractere necesita si numarul de elemente maxim citit
		fscanf_s(f, "%s", aux->iban, sizeof(aux->iban));// fscanf_s pentru siruri si caractere necesita si numarul de elemente maxim citit
		fscanf_s(f, "%s", currency, sizeof(currency));// fscanf_s pentru siruri si caractere necesita si numarul de elemente maxim citit
		fscanf_s(f, "%g%*c", &aux->sold);// %*c se foloseste pentru a citi \n de la sfarsit de rand si face discard
		aux->currency = stringToCurrency(currency);
		aux->urm = NULL;
		q = SearchUserByCNP(head, cnp);
		if (!q) {
			free(aux);
			printf("Nu s-a gasit utilizatorul cu cnp %s\n", cnp);
			continue;
		}
		else {
			temp = SearchAccount(q->headAccount, aux->iban);
			if (temp) {
				printf("Contul cu IBAN %s exista deja\n", aux->iban);
				free(aux);
			}
			else {
				AddToEndAccount(q, aux);
			}
		}
	}
}

int main()
{
	struct User* head = NULL;// capul listei de utilizatori
	head = ReadUsersFromFile("users.txt");
	ReadAccountsFromFile(head, "accounts.txt");
	int opt;
	do {
		printf("\n0. Iesire");
		printf("\n1. Autenitifcare");
		printf("\nOptiunea aleasa: "); scanf_s("%d", &opt);
		if (opt == 1) {
			LoginUser(head);
			if (UserLogged) {
				if (UserLogged->userType == user) MeniuUser(head);
				else MeniuAdmin(head);
			}
		}


	} while (opt);
	ShowUsers(head);

}