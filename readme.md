Proiect parallel snake game

Jocul este unul simplu:
Un numar de serpi sunt pusi pe o harta si acestia se misca in funtie de directia specificata.

Avem o harta sub forma unei matrici, iar serpii sunt reprezentati prin numere.

Scopul proiectului este acela de a implementa functional jocul folosindu-ne de paralelism.

Programul primeste ca argumente un fisier de input, un fisier de output si un numar de runde.
Jocul se opreste cand numarul de runde s-a terminat sau cand 2 serpi au avut o coliziune.

In fisierul de intrare gasim urmatoarele date:

<nr_serpi>
<coordonata_x> < coordonata_y> <codul_sarpelui> <directia_de_deplasare> (pe <nr_serpi> linii)
<lungime_harta> <latime_harta>
<harta>

, unde codul unui sarpe este dat de un numar, directia de deplasare este reprezentata prin coordonate N V 
S E, iar harta este o matrice de lungimea si latimea specificata.

Recomand rularea jocului pe linux, iar intr-o consola sa se scrie urmatoarele linii:

$make
$./parallel_snake inputx outx <nr_runde>

Pentru corectare, se compara rezultatul obtinut in outx cu rezultatul din outputx, unde x poate fi 1 sau 2
 sau un fisier de test generat pe loc. 
