///////////////////////////////////////////////////////////////////////////////
//
//		Rouguelike
//
///////////////////////////////////////////////////////////////////////////////
/*
ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
＊マップを表示												
＊マップ内を移動できること									
＊階段でマップを階層を移動できること				
＊Goldを拾うことができること（ステータスのGoldが増えるのみ）
＊ステータス表示（Goldのみ）								
＊ステータス表示（HP）										
ーーーーーーーーーーーーーーーーーーーーーーーーーーーー



		 */
		 ///////////////////////////////////////////////////////////////////////////////
		 //	@		Rogue. このシンボルは君、冒険者を示している。
		 //	- |		Wall. これらは部屋の壁を示している。
		 //	+		Door. 部屋から/へのドア。
		 //	.		The floor of a room.
		 //	#		Floor. 部屋の床。(廊下)
		 //	*		Gold. 金塊の山、壺。
		 //	%		Staircase. 他の階への階段。「 > + ENTER 」で１階から２階へ
		 //	:		Food. 食料。
		 ///////////////////////////////////////////////////////////////////////////////
		 // スクリーンの最下行(現在の状態を示す)
		 //	Level
		 //	今いる階
		 //	Gold
		 //	集めた金塊の数。
		 //	Hp
		 //	休憩する事で、健康ポイントは回復する事ができる。
		 //	括弧内の数値は、最大値。
		 //	Str
		 //強さ（未実装）
		 //	Arm
		 //	守備力（未実装）
		 //	Exp
		 //経験値（未実装）
		 //////////////////////////////////////////////////////////////////////////////////////////////////////
		 // コマンド+ENTER
		 // h	左へ移動
		 // j	下へ移動
		 // k	上へ移動
		 // l	右へ移動
		 // .	何もしない（休憩：体力の回復）
		 // >	go down a staircase. 次の階への階段を下りる。階段の所に立っている時しか下りる事はできない。
		 // Q	quit. 終了。
		 //////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <conio.h>

using	namespace	std;

class Player {
private:
	int	x;
	int	y;
	int	symbol;
	int	gold;
	int	amulet;
	int foods;
	int stomach;
	int	hp;
	int	maxhp;
	int	usehp;
	string	msg;
	bool	dead;
public:
	Player(int symbol)
	{
		stomach = 1000;
		foods = 0;
		amulet = 0;
		gold = 0;
		x = 0;
		y = 0;
		hp = 14;
		maxhp = 14;
		this->symbol = symbol;
		dead = false;
		usehp = 0;
	}
	void	rest()
	{
		if (usehp>0)
			usehp--;
	}
	void	useHp()
	{
		usehp++;
		if (usehp > 30)
		{	//30回以上移動するとhpが1減る
			usehp = 0;
			hp--;
			if (hp <= 0) {
				dead = true;
			}
		}
	}
	bool	isDead()
	{
		return	dead;
	}
	void	eatFood()
	{
		if (foods > 0) {
			foods--;
			stomach += 200;
		}
		else {
			msg = "食料がない";
		}
	}
	void	addFood()
	{
		foods++;
	}
	/*void	addAmulet()
	{
		amulet++;
	}*/
	/*bool	hasAmulet()
	{
		if (amulet > 0) {
			return	true;
		}
		return	false;
	}*/
	void	addGold()
	{
		gold++;
	}
	void	setPosi(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void	setMessage(const char *msg) {
		this->msg = msg;
	}
	int	getX() {
		return x;
	}
	int	getY() {
		return y;
	}
	int	getSymbol() {
		return symbol;
	}
	void move(int addX, int addY) {
		x += addX;
		y += addY;
		stomach--;
		if (stomach <= 0) {
			dead = true;
		}
		useHp();
	}
	void disp(int floorNumber)
	{
		char	buf[128];
		sprintf(buf, "階:%2d 金塊:%4d 体力:%2d(%2d) 強さ: %2d(%2d) 守備: %2d(%2d) 経験: %2d(%2d) ",
			floorNumber,
			gold,
			hp,
			maxhp,
			0,
			0,
			0,
			0,
			0,
			0);
		cout << buf;
		if (stomach < 50) {
			cout << "飢餓" << endl;
		}
		else if (stomach < 100) {
			cout << "く空腹" << endl;
		}
		else {
			cout << endl;
		}
		cout << msg << endl;
		msg = "";
	}
};


class Floor {
private:
	vector<string> map;
	int		player_x;
	int		player_y;
public:
	Floor()
	{
		player_x = 0;
		player_y = 0;
	}
	void	setPlayerPosi(Player & player)
	{
		player.setPosi(player_x, player_y);
	}
	bool	load(const char *fname)
	{
		std::ifstream ifs(fname);
		if (!ifs) {
			return	false;
		}
		std::string buf;
		int		i = 0;
		while (getline(ifs, buf)) {
			int pos = buf.find("@");
			if (pos != string::npos)
			{
				buf[pos] = '.';
				player_x = pos;
				player_y = i;
			}
			map.push_back(buf);
			i++;
		}
		return	true;
	}
	void	disp(Player & player)
	{
		system("cls");
		int	temp = map[player.getY()][player.getX()];
		map[player.getY()][player.getX()] = player.getSymbol();
		for (int i = 0;i<map.size();i++) {
			std::cout << map[i] << std::endl;
		}
		map[player.getY()][player.getX()] = temp;
	}
	bool	isMove(int x, int y)
	{
		if (map[y][x] != '|'
			&& map[y][x] != '-'
			&& map[y][x] != ' ') {
			return	true;
		}
		return	false;
	}
	void	action(Player & player)
	{
		// Item/Goldなどを拾う、モンスターの場合は戦闘に。
		int	temp = map[player.getY()][player.getX()];
		switch (temp) {
		case '*':		// GOLDを拾う
			player.addGold();
			map[player.getY()][player.getX()] = '.';
			player.setMessage("金塊を拾った");
			break;
		/*case ',':		// イェンダーの魔除けを拾う
						// playerがイェンダーの魔除けを持つようなメンバ関数を追加
			player.addAmulet();
			map[player.getY()][player.getX()] = '.';
			player.setMessage("イェンダーの魔除けを拾った");
			break;*/
		case ':':	// food
			player.addFood();
			map[player.getY()][player.getX()] = '.';
			player.setMessage("食料を拾った");
			break;
		}
	}
	int	getMap(Player & player)
	{
		return map[player.getY()][player.getX()];
	}
	int	setMap(Player & player, int item)
	{
		map[player.getY()][player.getX()] = item;
	}
};


class Dungeon {
private:
	vector<Floor> floors;
	int		currentFloor;
public:
	Dungeon()
	{
		// 複数階を読み込む
		for (int i = 1;;i++) {
			Floor	floor;
			char	buf[128];
			sprintf(buf, "dungeon%02d.txt", i);
			if (!floor.load(buf)) {
				break;
			}
			floors.push_back(floor);
		}
		currentFloor = 0;
	}
	int		getFloorNumber()
	{
		return currentFloor + 1;
	}
	void	setPlayerPosi(Player & player)
	{
		floors[currentFloor].setPlayerPosi(player);
	}
	void	disp(Player & player)
	{
		floors[currentFloor].disp(player);
	}
	bool	isMove(int x, int y)
	{
		return	floors[currentFloor].isMove(x, y);
	}
	void	action(Player & player)
	{
		floors[currentFloor].action(player);
	}
	void	goUp(Player & player)
	{
		// イェンダーの魔除けを所有していない者は、上る事ができない。
		/*if (!player.hasAmulet()) {
			return;
		}
		else {
			player.setMessage("イェンダーの魔除けが見つかりません");
		}*/
		int item = floors[currentFloor].getMap(player);
		if (item == '%')
		{
			if (currentFloor > 0) {
				currentFloor--;
				floors[currentFloor].setPlayerPosi(player);
			}
			else {
				// Goal !!(地上)
			}
		}
		else {
			player.setMessage("階段が見つかりません");
		}
	}
	void	goDown(Player & player)
	{
		int item = floors[currentFloor].getMap(player);
		if (item == '%')
		{
			if (floors.size() - 1 > currentFloor) {
				currentFloor++;
				// Plyerの座標を変える
				floors[currentFloor].setPlayerPosi(player);
			}
		}
		else {
			player.setMessage("階段が見つかりません");
		}
	}
};

bool	inputCommand(int &add_x, int &add_y, int &cmd)
{
	string	command;
	add_x = 0;
	add_y = 0;
	cmd = 0;

	cin >> command;
	if (command == "Q") {
		return false;
	}
	else if (command == ".") {
		cmd = 4;				// rest
	}
	else if (command == "e") {
		cmd = 3;				// eat food
	}
	else if (command == "<") {
		cmd = 2;				// go up
	}
	else if (command == ">") {
		cmd = 1;				// go down
	}
	else if (command == "h") {
		add_x = -1;
	}
	else if (command == "j") {
		add_y = 1;
	}
	else if (command == "k") {
		add_y = -1;
	}
	else if (command == "l") {
		add_x = 1;
	}
	return true;
}

int main()
{
	Dungeon	dungeon;
	Player	player('@');
	int		add_x = 0;
	int		add_y = 0;
	int		cmd = 0;

	dungeon.setPlayerPosi(player);
	while (1) {
		dungeon.disp(player);
		player.disp(dungeon.getFloorNumber());
		if (!inputCommand(add_x, add_y, cmd)) {
			break;
		}
		if (cmd == 0) {
			if (dungeon.isMove(player.getX() + add_x, player.getY() + add_y)) {
				player.move(add_x, add_y);
				dungeon.action(player);
			}
		}
		else if (cmd == 1) {
			dungeon.goDown(player);
		}
		else if (cmd == 2) {
			dungeon.goUp(player);
		}
		else if (cmd == 3) {
			player.eatFood();
		}
		else if (cmd == 4) {
			player.rest();
		}
		if (player.isDead())
		{
			// GameOver
			system("cls");
			cout << "Game Over!!" << endl;
			break;
		}
	}
	return 0;
}