# proiect_micro_FRDM_KL25Z128
Dumitru Andreea-Ioana
Mocanu Razvan

	Descriere proiect:
Scopul proiectului este acela de a modifica unghiul de rotație al servomotorului utilizând un senzor de sunet printr-un semnal PWN generat și manipulat în funcție de plaja de valori a senzorului de sunet.
Valoarea digitală care va rezulta în urma conversiei din tensiune a datelor achiziționate de la senzorul de sunet se vor regăsi într-un interval care va fi apoi împărțit in trei subintervale care vor corespunde cu valori mici, medii și mari ale datelor achiziționate.
În funcție de subintervalul în care se regăsesc datele convertite din analogic în digital, servomotorul își va schimba poziția elicei (0°, 90° și 180°).

	Conectare senzori:
Senzorul de sunet a fost conectat pe plăcuță astfel:
 -Firul roșu (VCC) se conectează la pinul de 3V (P3V3);
 -Firul negru (GND) se conectează la pinul GND;
 -Firul albastru (output sensor value) se conectează la pinul PTC2.

Servomotorul a fost conectat pe plăcuță astfel:
 -Firul maroniu (GND) se conectează la pinul GND;
 -Firul roșu (VCC) se conectează la pinul de 5V (P5V_USB) ;
 -Firul portocaliu (PWN) se conectează la PTB2.

	Rulare:
Se compilează proiectul din qVision făcând Build la cod urmat de Load pe plăcuță, iar apoi din Python se pornește intefața grafică unde se introduce portul pe care este conectat target-ul și se apasă butonul ”Start Recording”.
În grafic, se vor plota 3 barplot-uri ce reprezintă cele 3 intervale în care sunt valorile primite de la senzorul de sunet, ce sunt actualizate în mod dinamic.
Pentru schimbarea secvenței GPIO de LED-uri, se va apăsa pe butonul ”Switch” ceea ce va face inversare din RGB în BGR.

	Link Video:
https://drive.google.com/file/d/1AV2U4tmoP9WBCagUG5dxubkQGCuGewPP/view?usp=drive_link
