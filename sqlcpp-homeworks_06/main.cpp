#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <iostream>
#include <string>
#include <Windows.h>

#pragma execution_charaster_set( "utf-8")

using namespace std;
class Book;
class Stock;
class Sale;

//»здатель
class Publisher {
public:
	string name = "";

	template <class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
	}

};

// нига//ќдна id книга-ќдин id издатель
class Book {
public:
	string title = "";
	//link to publisher
	Wt::Dbo::ptr<Publisher> publisher_id;

	template <class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, title, "title");
		//link to publisher;
		Wt::Dbo::belongsTo(a, publisher_id, "publisher");
	}
};

//ћагазин
class Shop {
public:
	string name = "";

	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
	}
};

//—ток// дл€ одной строки id —тока продажи в одном id ћагазине одной id  ниги
class Stock {
public:
	//link to book;
	Wt::Dbo::ptr<Book> book_id;
	//link to shop
	Wt::Dbo::ptr<Shop> shop_id;
	int count;

	template<class Action>
	void persist(Action& a) {
		//link to book;
		Wt::Dbo::belongsTo(a, book_id, "book");
		//link to shop;
		Wt::Dbo::belongsTo(a, shop_id, "shop");
		Wt::Dbo::field(a, count, "count");
	}
};

//продажи // дл€ одной строки id ѕродаж один id —ток
class Sale {
public:
	double price = 0.0;
	string date_sale = "";
	//link to Stock
	Wt::Dbo::ptr<Stock> stock_id;
	int count = 0;

	template<class Action>
	void persist(Action a) {
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		//link to Stock;
		Wt::Dbo::belongsTo(a, stock_id, "stock");
		Wt::Dbo::field(a, count, "count");
	}
};

int main() {
	//setlocale(LC_ALL, "rus");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	try {
		//подключаемс€ к postgresDB
		cout << "Hello!" << endl;
		string connectionString =
			"host=localhost "
			"port=5432 "
			"dbname=book_shops "
			"user=postgres "
			"password=33995062";

		auto postgres = make_unique<Wt::Dbo::backend::Postgres>(connectionString);
		Wt::Dbo::Session session;
		session.setConnection(move(postgres));

		//создаем таблицы
		session.mapClass<Publisher>("publisher");
		session.mapClass<Book>("book");
		session.mapClass<Shop>("shop");
		session.mapClass<Stock>("stock");
		session.mapClass<Sale>("sale");

		try {
			session.createTables();
		}
		catch (...) {
		}

		//заполн€ем Publisher
		
			unique_ptr<Publisher> pehov{ new Publisher() };
			pehov->name = "A.Pehov";

			unique_ptr<Publisher> king{ new Publisher() };
			king->name = "S.King";

			unique_ptr<Publisher> lovecraft{ new Publisher{"H.Lovecraft"} };

			Wt::Dbo::Transaction tr{ session };
			auto pehovPtr = session.add<Publisher>(move(pehov));
			auto kingPtr = session.add<Publisher>(move(king));
			auto lovecraftPtr= session.add<Publisher>(move(lovecraft));

			tr.commit();

		//заполн€ем book
			unique_ptr<Book> pehov1{ new Book{"Sneeking in shadows", pehovPtr} };
			unique_ptr<Book> pehov2{ new Book {"Dzhanga with shadows", pehovPtr} };
			unique_ptr<Book> pehov3{ new Book{"Strom of shadows", pehovPtr} };
			unique_ptr<Book> pehov4{ new Book{"Wind seekers", pehovPtr} };
			unique_ptr<Book> king1{ new Book{"IT", kingPtr} };
			unique_ptr<Book> king2{ new Book{"IT pt.2", kingPtr} };
			unique_ptr<Book> king3{ new Book{"Kristine", kingPtr} };
			unique_ptr<Book> lovecraft1{ new Book{"The call of Cthulhu", lovecraftPtr} };
			unique_ptr<Book> lovecraft2{ new Book{"Mountains of madness", lovecraftPtr} };
			unique_ptr<Book> lovecraft3{ new Book{"Whisperer in the darkness", lovecraftPtr} };

			Wt::Dbo::Transaction tr1{ session };

			auto peh_b_ptr1 = session.add<Book>(move(pehov1));
			auto peh_b_ptr2 =  session.add<Book>(move(pehov2));
			auto peh_b_ptr3 = session.add<Book>(move(pehov3));
			auto peh_b_ptr4 = session.add<Book>(move(pehov4));
			auto king_b_ptr1 = session.add<Book>(move(king1));
			auto king_b_ptr2 = session.add<Book>(move(king2));
			auto king_b_ptr3 = session.add<Book>(move(king3));
			auto lc_b_ptr1 = session.add<Book>(move(lovecraft1));
			auto lc_b_ptr2 = session.add<Book>(move(lovecraft2));
			auto lc_b_ptr3 = session.add<Book>(move(lovecraft3));

			tr1.commit();
		
		//заполн€ем магазины
		
			Wt::Dbo::Transaction tr2{ session };
			unique_ptr<Shop> shop1{ new Shop{"Classic fanasy"} };
			unique_ptr<Shop> shop2{ new Shop{"Horrors and adventure"} };
			unique_ptr<Shop> shop3{ new Shop{"Library of best"} };

			auto shop1Ptr = session.add<Shop>(move(shop1));
			auto shop2Ptr = session.add<Shop>(move(shop2));
			auto shop3Ptr = session.add<Shop>(move(shop3));
			tr2.commit();
		

		//заполн€ем Stock
		
			Wt::Dbo::Transaction tr3{ session };
			//магазин 1 пехов 1
			unique_ptr<Stock> stock_p1_sh1{ new Stock{peh_b_ptr1,shop1Ptr, 50} };		
			//магазин 2 пехов 1
			unique_ptr<Stock> stock_p1_sh2{ new Stock{peh_b_ptr1,shop2Ptr, 10} };
			//магазин 3 пехов 2
			unique_ptr<Stock> stock_p2_sh3{ new Stock{peh_b_ptr2,shop3Ptr, 23} };
		
			//магазин 2 кинг 3
			unique_ptr<Stock> stock_k3_sh2{ new Stock{king_b_ptr3,shop2Ptr, 32} };
			//магазин 3 кинг 2
			unique_ptr<Stock> stock_k2_sh3{ new Stock{king_b_ptr2,shop3Ptr, 13} };

			//магазин 2 лавкрафт 1
			unique_ptr<Stock> stock_lc1_sh2{ new Stock{lc_b_ptr1,shop2Ptr, 84} };

			//пехов книги во всех (трех) магазинах
			auto stock_ptr1 = session.add<Stock>(move(stock_p1_sh1)); 
			auto stock_ptr2 = session.add<Stock>(move(stock_p1_sh2));
			auto stock_ptr3 = session.add<Stock>(move(stock_p2_sh3));
			//кинг магазины номер 2 и 3
			auto stock_ptr4 = session.add<Stock>(move(stock_k3_sh2));
			auto stock_ptr5 = session.add<Stock>(move(stock_k2_sh3));
			//лавкрафт магазин номер 2
			auto stock_ptr6 = session.add<Stock>(move(stock_lc1_sh2));

			tr3.commit();

		//заполн€ем Sale
			unique_ptr<Sale> sale_pehov_sh1{ new Sale{250,"2023-12-04",stock_ptr1,3} };
			unique_ptr<Sale> sale_pehov_sh1_1{ new Sale{250,"2023-12-05",stock_ptr1,5} };
			unique_ptr<Sale> sale_pehov_sh2{ new Sale{280,"2023-12-05",stock_ptr2,8} };
			unique_ptr<Sale> sale_pehov_sh3{ new Sale{265,"2023-12-05",stock_ptr3,13} };
			unique_ptr<Sale> sale_king_sh2{ new Sale{350,"2023-12-05",stock_ptr4,2} };
			unique_ptr<Sale> sale_king_sh3{ new Sale{400,"2023-12-05",stock_ptr5,6} };
			unique_ptr<Sale> sale_lc_sh2{ new Sale{600,"2023-12-05",stock_ptr6,1} };


			Wt::Dbo::Transaction tr4{ session };
			session.add<Sale>(move(sale_pehov_sh1));
			session.add<Sale>(move(sale_pehov_sh1_1));
			session.add<Sale>(move(sale_pehov_sh2));
			session.add<Sale>(move(sale_pehov_sh3));
			session.add<Sale>(move(sale_king_sh2));
			session.add<Sale>(move(sale_king_sh3));
			session.add<Sale>(move(sale_lc_sh2));

			tr4.commit();

			string user_input;
			std::cout << "Enter name or id of publisher" << endl;
			std::cin >> user_input;

			Wt::Dbo::Transaction tr5{ session };
			//определ€ем id искомого ѕаблишера
			auto publisher_id_founded = session.find<Publisher>().where("id=?").bind(user_input).orWhere("name=?").bind(user_input).resultValue();
			////находим список книг паблишера
			auto publisher_books_list = session.find<Book>().where("publisher_id = ?").bind(publisher_id_founded);

			/*for (auto& x : publisher_books_list.resultList()) {
				cout << x->title << endl;
			}*/

			//список стоков, где есть книги ѕаблишера			
			auto stock_id_list_with_publisher_books = session.find<Stock>().where("book_id IN (?)").bindSubqueryValues(publisher_books_list);
			for (auto& x : stock_id_list_with_publisher_books.resultList()) {
				cout << x->book_id << endl;
			}
			//получаем список  стоков, где были продажи среди ранее полученного списка стоков
			auto stock_id_active_sale_with_publisher_books = session.find<Sale>().where("stock_id IN (?)").bindSubqueryValues(stock_id_list_with_publisher_books);
	
			//по стоку находим список магазинов, где продавались 
			auto shop_id_list = session.find<Stock>().where("id IN (?)").bindSubqueryValues(stock_id_active_sale_with_publisher_books).resultList();

			cout << "n\Books written by " << publisher_id_founded->name << ""
				" are selling in this shops:" << endl;
			for (auto& x : shop_id_list) {
				cout << x->shop_id->name << endl;
			}
			//

	}

	catch (std::exception& er) {
		cout << er.what();
	}


	return 0;
}