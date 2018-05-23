#include <iostream>

/*В данном примере рассматривается понятие шаблонного класса - то есть, класса, который работает вне зависимости от типа переменных. Шаблон - 
это описание класса для неизвестного типа переменной. Экземпляр определенного класса - это создание объекта класса с указанием в качестве параметра
определенного типа данных, который заменяет во всем описании класса шаблон на указанный тип. Так, класс matrix_t является шаблонным, поскольку может
работать в различными типами данных (в данном случае, являться матрицей для целых чисел, для вещественных чисел, для переменных типа bool и так далее).
Для объявления определенного экземпляря класса необходимо при объявлении типа писать не только название класса, но также указывать в угловых скобках тип
шаблона: например, matrix_t<int> - это создание экземпляра, где все вхождения типа T в описании класса становятся типами int, matrix_t<float> - типами 
float.
Для создания шаблонного класса необходимо объявить имя шаблонного типа. Так, имя T становится именем типа, такого как int, float и других возможных.
Во всем классе нужно использовать тип T для соответствующих переменных*/

//Такая конструкция объявляет шаблон типа с именем T
template <typename T>
class matrix_t {
private: //"кишки" класса
	T ** elements_; //элементы матрицы могут быть различными, это зависит от параметра, который будет передаваться классу при его объявлении
	std::size_t rows_; //количество строк матрицы - всегда целое число
	std::size_t collumns_; // количество столбцов матрицы - всегда целое число
public: //интерфейс класса
	
	//Явное создание конструктора - инициализация переменных
	matrix_t() {
		//Заметим, что для обращения к переменным исходного класса квалификатор this-> можно опускать и использовать непосредственно
		//имя необходимых переменных
		elements_ = nullptr; //матрица является пустой
		rows_ = 0; //0 строк
		collumns_ = 0; //0 столбцов
	}
	
	//Еще одна версия конструктора - конструктор с объекторм данного класса в качестве аргумента 
	//(он с квалификатором const, поскольку его нельзя изменять). Такой конструктор называется конструктором копирования
	matrix_t( matrix_t<T> const & other )
	{
		rows_ = other.rows_; //количество строк такое же, как у другого объекта
		collumns_ = other.collumns_; ////количество столбцов такое же, как у другого объекта
		
		//выделение памяти для заполнения матрицы элементами определенного типа
		elements_ = new T *[ rows_]; //поскольку используется выделение памяти через new, то потребуется создавать деструктор
		for (std::size_t i = 0; i <  rows_; ++i) {
		elements_[i] = new T [collumns_];
			for (std::size_t j = 0; j < collumns_; ++j) {
				elements_[i][j] = other.elements_[i][j];
			}
		}
	}
	
	//Оператор присвающего копирования - действует практически так же, как конструктор копирования, однако несколько отличается от него
	matrix_t<T> & operator =( matrix_t<T> const & other )
	{
		//Если исходный объект был заполненным, то изначально следует освободить память, которую он занимал, и лишь потом копировать новые
		//элементы. Это необходимо для предотворощения утечки памяти
		for (std::size_t i = 0; i < rows_; ++i) {
			delete[] elements_[i];
		}
		delete[] elements_;
		rows_ = other.rows_; //количество строк такое же, как у другого объекта
		collumns_ = other.collumns_; //количество столбцов такое же, как у другого объекта
		elements_ = new T * [rows_]; //выделение памяти для матрицы определенного значения и копирование соответствующих элементов, то есть, 
					     //такой же матрицы
		for (std::size_t i = 0; i < rows_; ++i) {
			elements_[i] = new T [collumns_];
			for (std::size_t j = 0; j < collumns_; ++j) {
				elements_[i][j] = other.elements_[i][j];
			}
		}			
		return *this; //возвращается исходный объект
	}

	//Деструктор. Поскольку использовалась операция new[], то для освобождения памяти необходимо использовать delete[]
	~matrix_t()
	{
		//Освобождение памяти, занимаемой матрицей
		for (std::size_t i = 0; i < rows_; i++) {
			delete [] elements_[i];
		}
		delete []elements_;
	}
    
	//Функция, которая возвращает количество строк
   	 std::size_t rows() const
	{
	    return rows_;
	}
	
	//Функция, которая возвращает количество столбцов
	    std::size_t collumns() const
	{
    		return collumns_;
	}

	//Оператор сложение - перегрузка операции сложения, чтобы можно было ее использовать для пользовательских классов
	matrix_t<T> operator +( matrix_t<T> const & other ) const
	{
		matrix_t<T> result; //создание матрицы с параметром T - таким же, которым обладает исходный объект
	
		//Проверка условия возможности сложения матриц. Если это возможно - действуем по формуле, иначе выводим исключение
		if (rows_ == other.rows_ && collumns_ == other.collumns_) {
		  	result.elements_ = new T *[rows_];
			for (std::size_t i = 0; i<rows_; i++) {
				result.elements_[i] = new T [collumns_];
			}
			result.rows_ = rows_;
			result.collumns_ = collumns_;
		
			for (std::size_t i = 0; i<rows_; i++) {
				for (std::size_t j = 0; j<collumns_; j++) {
					result.elements_[i][j] = elements_[i][j] + other.elements_[i][j];
				}
			}
		}
		else {
			//Исключение - это ошибка, которую может сгенерировать сам программист. То есть, программа не аварийно завершится,
			//а сделает это ожидаемо и выдаст сообщение об ошибке с определенными программистом кодами или описания ошибки
			// trow - ключевое слово, которое генерирует исключение, то есть, указывает на ошибку и завершает программу
			throw std::invalid_argument("Invalid syntax");
		}

		return result;
	}
	
	//Оператор вычитания аналогично перегружается для использования операции вычитания для определенных пользователем типов данных
	matrix_t<T> operator -( matrix_t<T> const & other ) const
	{
		matrix_t<T> result;
	
		if (rows_ == other.rows_ && collumns_ == other.collumns_) {
		  	result.elements_ = new T *[rows_];
			for (std::size_t i = 0; i<rows_; i++) {
				result.elements_[i] = new T [collumns_];
			}
			result.rows_ = rows_;
			result.collumns_ = collumns_;
		
			for (std::size_t i = 0; i<rows_; i++) {
				for (std::size_t j = 0; j<collumns_; j++) {
					result.elements_[i][j] = elements_[i][j] - other.elements_[i][j];
				}
			}
		}
		else {
			throw std::invalid_argument("Invalid syntax");
		}

		return result;
	}
	
	//Аналогично оператор умножения
	matrix_t<T> operator *( matrix_t<T> const & other ) const
	{
		matrix_t<T> result;
		
		//Проверка условия возможности произведения матриц и создание результирующей  матрицы определенного размера в случае успеха
		if (collumns_ == other.rows_) {
  			result.elements_ = new T *[rows_];
			for (std::size_t i = 0; i<rows_; i++) {
				result.elements_[i] = new T [other.collumns_];
		    }
		    result.rows_ = rows_;
	    	result.collumns_ = other.collumns_;
	    
			for (std::size_t i = 0; i < rows_; ++i) {
				for (std::size_t j = 0; j < other.collumns_; ++j) {
					int result_ = 0;
					for (std::size_t k = 0; k < other.rows_; ++k) {
						result_ += elements_[i][k] * other.elements_[k][j];
					}
					result.elements_[i][j] = result_;
				}
			}
		}
		else {
			//Если операция невозможно, генерируется сообщение об ошибке
			throw std::invalid_argument("Invalid syntax");
		}

		return result;
	}

	//Перегрузка оператора -= - присваивание результата вычитания одному из используемых в операции объектов
	matrix_t<T> & operator -=( matrix_t<T> const & other )
	{
		if (rows_ == other.rows_ && collumns_ == other.collumns_) {
			for (std::size_t i = 0; i<rows_; i++) {
				for (std::size_t j = 0; j<collumns_; j++) {
					elements_[i][j] -= other.elements_[i][j];
				}
			}
		}
		else {
			throw std::invalid_argument("Invalid syntax");
		}
	
		return *this; //сразу изменяется исходная матрица без создания новой
	}
	
	//Аналогично для +=
	matrix_t<T> & operator +=( matrix_t<T> const & other )
	{
		if (rows_ == other.rows_ && collumns_ == other.collumns_) {
			for (std::size_t i = 0; i<rows_; i++) {
				for (std::size_t j = 0; j<collumns_; j++) {
					elements_[i][j] += other.elements_[i][j];
				}
			}
		}
		else {
			throw std::invalid_argument("Invalid syntax");
		}
	
		return *this;
	}
	
	//Операция произведения требует создания вспомогательной матрицы, однако в остальном действует аналогично
	matrix_t<T> & operator *=( matrix_t<T> const & other )
	{
		matrix_t<T> result;
	
		if (collumns_ == other.rows_) {
			result.elements_ = new T *[rows_];
			for (std::size_t i = 0; i<rows_; i++) {
				result.elements_[i] = new T [other.collumns_];
		    }
		    result.rows_ = rows_;
	    	result.collumns_ = other.collumns_;
	    
			for (std::size_t i = 0; i < rows_; ++i) {
				for (std::size_t j = 0; j < other.collumns_; ++j) {
					int result_ = 0;
					for (std::size_t k = 0; k < other.rows_; ++k) {
						result_ += elements_[i][k] * other.elements_[k][j];
					}
					result.elements_[i][j] = result_;
				}
			}
			*this = result;
		}
		else {
			throw std::invalid_argument("Invalid syntax");
		}
	
		return *this;
	}

	//Функция, позволяющая считать матрицу с консоли
	std::istream & read( std::istream & stream )
	{
		    std::size_t rows;
		    std::size_t collumns;
	    	char symbol;
    	
		    bool success = true;
		
		//Если матрица считывается верно, то заполняется исходный объект
	    	if( stream >> rows && stream >> symbol && symbol == ',' && stream >> collumns ) {
    		    T ** elements = new T *[ rows ];
        		for( std::size_t i = 0; success && i < rows; ++i ) {
		            elements[ i ] = new T[ collumns ];
        		    for( std::size_t j = 0; j < collumns; ++j ) {
                		if( !( stream >> elements[ i ][ j ] ) ) {
     	    	           success = false;
        		            break;
                		}
       		     	}
        		}
        
		        if( success ) {
        		    for( std::size_t i = 0; i < rows_; ++i ) {
                		delete [] elements_[ i ];
            		}
           		 delete [] elements_;
            
   	        	 rows_ = rows;
	        	    collumns_ = collumns;
       		     elements_ = elements;
       		 }
       		 else {
        		    for( std::size_t i = 0; i < rows; ++i ) {
                		delete [] elements[ i ];
   	         	}
  	      	   	 delete [] elements;
      	 	 }
    		}
   		 else {
        		success = false;
    		}
    
 	 	  if( !success ) {
        		stream.setstate( std::ios_base::failbit );
    		}
    
		return stream;
	}

	//Функция, позволяющая отобразить получившуюся матрицу на мониторе компьютера
	std::ostream & write( std::ostream & stream ) const
	{
		    stream << rows_ << ", " << collumns_;
		    for( std::size_t i = 0; i < rows_; ++i ) {
			stream << '\n';
		        for( std::size_t j = 0; j < collumns_; ++j ) {
    		    	    stream << elements_[ i ][ j ];
      		      if( j != collumns_ - 1 ) {
        		        stream << ' ';
     		       }
    	 	   }
    		}
   		 if (collumns_ != rows_) stream << '\n';
    
			return stream;
	}
	};
