/*Данный файл служит тестирующей проверкой правильности посторения программы. То есть, здесь проверяются определенные функции класса,
проверяется правильность их выполнения, граничные случаи и поведение программы в случае ошибки*/

#include <cassert>
#include <catch.hpp>
#include <sstream>

#include "matrix.hpp" //подключение файла с реализацией программы

/*Используются так называемые макросы. В скобочках TEST_CASE пишется название теста, которое должно быть уникальным для каждой проверки.
Далее идет стандартная программа на Си++, с объявлением переменных и определенными методами. В конце проверяются выходные данные с ожидаемыми
посредством макроса REQUIRE, где проверяется логическое выражение. В случае REQUIRE(true) программа работает правильно и ожидаемое значение
совпадает с реальным; в случае REQUIRE(false) поограмма работает неверно и выводится сообщение об ошибке*/

TEST_CASE("creating matrix") //создание теста
{
    matrix_t<int> matrix; //создание матрицы с элементами типа целых чисел
    REQUIRE( matrix.rows() == 0 ); //так как должен использоваться коструктор без аргументов, то значение строк и столбцов должно быть 0
    REQUIRE( matrix.collumns() == 0 ); //через возвращаемые значения количества строк и столбцов и сравнения их с 0 получаем либо true, либо false
    //так как после выполнения всех операторов блок программы завершается и, соответственно, завершается область видимости matrix, то в данном
    //месте для нее вызывается деструктор
}

TEST_CASE("reading matrix")
{
    std::string input{ //создание строки, содержащей значение количесва строк, столбцов и самих ячеек матрицы
        "3, 3\n"
        "1 1 1\n"
        "2 2 2\n"
        "3 3 3" };
    matrix_t<int> matrix; //создание матрицы с элементами типа целых чисел
    std::istringstream stream{ input };
    
    REQUIRE( matrix.read( stream ) ); //инициализация матрицы через значения, введеные на консоль. Возвращаемое значение преобразуется в true либо false
    REQUIRE( matrix.rows() == 3 ); //сравнение количества строк и столбцов с ожидаемыми значениями
    REQUIRE( matrix.collumns() == 3 );
    
    std::ostringstream output;
    matrix.write( output ); //инициализация потока значением вывода матрицы на экран
    
    REQUIRE( input == output.str() ); //сравнение вывода матрицы с ожидаемым выводом программы
    //здесь вызывается деструктор
}

//Аналогичный пример, однако теперь создается экземпляр матрицы с вещественными числами
TEST_CASE("reading float_matrix")
{
    std::string input{
        "3, 3\n"
        "1.1 1.2 1.3\n"
        "2.4 2.5 2.6\n"
        "3.7 3.8 3.9" };
    matrix_t<float> matrix;
    std::istringstream stream{ input };
    
    REQUIRE( matrix.read( stream ) );
    REQUIRE( matrix.rows() == 3 );
    REQUIRE( matrix.collumns() == 3 );
    
    std::ostringstream output;
    matrix.write( output );
    
    REQUIRE( input == output.str() );
}

//Проверка правильности сложения матриц
TEST_CASE("addings matrixs")
{
    //Создаются и инициализируются строками с консоли две матрицы вещественного типа
    std::string first_matrix_representation{
        "1, 3\n"
        "1.1 1.1 1.1\n"
    };
    std::string second_matrix_representation{
        "1, 3\n"
        "1.1 1.1 1.1\n"
    };
    
    std::istringstream stream{ first_matrix_representation };
    matrix_t<float> first_matrix; 
    first_matrix.read( stream );
    
    std::istringstream stream_ { second_matrix_representation };
    matrix_t<float> second_matrix;
    second_matrix.read( stream_ );
    
    matrix_t<float> result_matrix = first_matrix + second_matrix; //создается матрица, здесь так же используется оператор копирующего присваивания

    std::string expected_result_matrix_representation{
        "1, 3\n"
        "2.2 2.2 2.2\n"
    };
    
    std::ostringstream output;
    result_matrix.write (output);
    
    REQUIRE( output.str() == expected_result_matrix_representation ); //сравнение вывода программы с ожидаемой получившейся матрицей
}

//Аналогично для разности матриц
TEST_CASE("subtracting matrixs")
{
    std::string first_matrix_representation{
        "1, 3\n"
        "1.5 1.5 1.5\n"
    };
    std::string second_matrix_representation{
        "1, 3\n"
        "1 1 1\n"
    };
    
    std::istringstream stream{ first_matrix_representation };
    matrix_t<float> first_matrix;
    first_matrix.read( stream );
    
    std::istringstream stream_ { second_matrix_representation };
    matrix_t<float> second_matrix;
    second_matrix.read( stream_ );
    
    matrix_t<float> result_matrix = first_matrix - second_matrix;

    std::string expected_result_matrix_representation{
        "1, 3\n"
        "0.5 0.5 0.5\n"
    };
    
    std::ostringstream output;
    result_matrix.write (output);
    
    REQUIRE( output.str() == expected_result_matrix_representation );
}

//Аналогично для произведения
TEST_CASE("multiplying matrixs")
{
    std::string first_matrix_representation{
        "1, 3\n"
        "1 1 1\n"
    };
    std::string second_matrix_representation{
        "3, 1\n"
        "1\n"
        "1\n"
        "1\n"
    };
    
    std::istringstream stream{ first_matrix_representation };
    matrix_t<int> first_matrix;
    first_matrix.read( stream );
    
    std::istringstream stream_ { second_matrix_representation };
    matrix_t<int> second_matrix;
    second_matrix.read( stream_ );
    
    matrix_t<int> result_matrix = first_matrix * second_matrix;

    std::string expected_result_matrix_representation{
        "1, 1\n"
        "3"
    };
    
    std::ostringstream output;
    result_matrix.write (output);
    
    REQUIRE( output.str() == expected_result_matrix_representation );
}

//Проверка генерации исключения
TEST_CASE("experiment")
{
    //Создаются две матрицы, которые не могут умножаться по определению
    std::string first_matrix_representation{
        "1, 1\n"
        "1.1\n"
    };
    std::string second_matrix_representation{
        "2, 1\n"
        "1.0\n"
        "1.0\n"
    };
    
    std::istringstream stream{ first_matrix_representation };
    matrix_t<float> first_matrix;
    first_matrix.read( stream );
    
    std::istringstream stream_ { second_matrix_representation };
    matrix_t<float> second_matrix;
    second_matrix.read( stream_ );
    
    //Макрос REQUIRE_THROWS_AS проверяет, какое именно исключение было сгенерировано во время выполнения функции
    //Так как умножение матрицы сгенерирует исключение, то если сгенерированное исключение будет таким же, как ожидаемое, то макрос сгенерирует
    //true и программа завершится правильно. В противном случае она завершится аварийно
    REQUIRE_THROWS_AS( (first_matrix * second_matrix ), std::invalid_argument);
}
