#include <vector>
class Player;
class Board;

class Card
{
private:
    /* data */
public:
    virtual ~Card() = default; // Butuh for dynamically allocated cards nanti
    virtual void onDraw(Player& p, Board& b, std::vector<Player>& all) = 0;
};

/**
 * Virtual void onDraw(Player& p, Board& b, std::vector<Player>& all)
 * memang redundan karena tiap memanggil butuh ngasih semua argumen itu.
 * 
 * Contoh, mungkin ada card A yang efeknya simpel, dan ga butuh tau board,
 * tapi dalam onDraw nya tetep harus pass boardnya. Tapi ini ga masalah karena
 * nanti yang manggil onDraw() harusnya di game manager atau entitas lain yang
 * secara default pass in "onDraw(Player& p, Board& b, std::vector<Player>& all);""
 * Urusan nanti argumennya dipake apa engga, tergantung masing-masing cards.
 * 
 * Aku pake approach ini soalnya biar card ga megang reference ke player atau board
 * cuz that leads to banyak masalah circular dependancy and also risiko dangling
 * pointers.
 *                                                                      -Billie
 */