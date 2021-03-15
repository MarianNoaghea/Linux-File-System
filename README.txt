Sistemul de fisiere este gandit in felul urmator: directorul de baza este root
de acesta avand 2 liste: una de fisiere si una de directoare, directoarele sunt 
de acelasi tip ca si rootul si la randul lor au 2 liste, si asa mai departe.
Fiecare linie citita de la tastatura este "maruntita" in cuvinte cu ajutorul
strtok apoi pusa intr-un vector de cuvinte, fiecare element din vector fiind
un argument in functie, uneori adresa vectorului este argument.
Pentru functiile touch si mkdir fiecare element din lista a fost introdus
in ordine alfabetica. Pentru ls s-au folosit doua functii de afisare a celor 2
liste(in ordinea ceruta). {entru pwd s-a parcurs ierarhia dinspre directorul
curent inspre root, s-a salvat fiecare element intr-un vector de cuvinte apoi
s-au afisat in ordine inversa.Pentru tree s-a afisat si s-a inaintat in ierarhia
directoarelor intr-un mod recursiv, tinandu-se cont la fiecare pas de cate ori 
s-a inaintat. Pentru rmdir si rm forma este una asemanatoare, totusi din cauza 
diferentelor de forma a celor 2 stringuri de nume, a fost nevoie de o abordare 
diferita pentru fiecare dintre acestea.
La eliberarea totala de memorie am intampinat o problema si am explicat-o si
in comentariile din cod.
