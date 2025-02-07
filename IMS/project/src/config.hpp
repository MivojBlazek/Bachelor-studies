#ifndef CONFIG_HPP
#define CONFIG_HPP

#define POCET_ZAMESTNANCU 34000
#define POCET_ZAMESTNANCU_V_DESTI 17000
#define PLAT 15
#define PROVOZNI_VYDAJE 7500 + 45000

#define CAS_OTEVRENI 9.5
#define CAS_UZAVRENI 22
#define POCET_DNI 365

#define ATRAKCE3_KAPACITA 10

/** Pocet navstevniku je 100krat mensi z duvodu zmenseneho systemu,
 *  a tudiz veskere ceny jsou 100krat vyssi.
 */
#define PRIJMY_ZE_VSTUPU 84 * 100
#define PRIJMY_Z_NAPOJU 5 * 100
#define PRIJMY_Z_JIDLA 10 * 100
#define PRIJMY_Z_SUVENYRU 34 * 3 * 100

#define VYDAJE_NA_PRVNI_MARKETING 2740
#define VLIV_REKALMY 0.0317// 3.17 %
#define POCET_MARKETINGU 0


/** V teto kategorii lze aktivovat jednotlive definice */

#define DEBUG
// #define UBRAT_ZAMESTNANCE_BEHEM_DESTE


#endif // CONFIG_HPP