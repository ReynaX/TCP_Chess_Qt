# Chess 
Aplikacja do grania w szachy wykorzystująca gniazda BSD do komunikacji się między graczami i serwer zarządzający połączeniami między nimi. Serwer jak i klient napisane są w C++ z wykorzystaniem frameworka Qt. 

## Logika szachów
Logika szachów opiera się na planszy składającej się z 64 przycisków. Każdy przycisk ma przypisany kolor i ikonkę zależnie od tego czy figura znajduje się na danym polu.
Po naciśnięciu na przycisk zawierający figurę tego samego koloru co którego kolor ma ruch, przyciski na które możliwy jest ruch zmienią swój kolor na kolor biały. Po nacisnięciu na któryś z przycisków, na które możliwy jest ruch, ruch zostanie wykonany. Po każdym ruchu sprawdzana jest obecna sytuacja na planszy, sprawdzając w pierwszej kolejności czy król koloru, który ma się właśnie poruszyć jest atakowany, co determinuje stan rozgrywki. Dodatkowo obliczane są możliwe ruchy tego koloru. Przy obliczeniu ruchów sprawdzane jest również czy usunięcie figury postawi króla w szachu.


## Logika komunikacji klient-serwer
Serwer po włączeniu nasłuchuje na połączenia na adresie 127.0.0.1 i porcie 1234. Po podłączenie się klienta, serwer nasłuchuje na wiadomości zawierające informacje do przetworzenia. Dane przekazywanie w komunikacji klient-serwer wyglądają nastepująco:

<code>TL<sub>1</sub>D<sub>1</sub>L<sub>2</sub>D<sub>2</sub>...</code>
gdzie:
- T - to typ informacji która została wysłana(host, join, disconnect itd.) reprezentowane przez jedną literę(najczęściej pierwszą literę operacji)
- L<sub>i</sub> - to długość kolejnego segmentu do przetworzenia
- D<sub>i</sub> - to dane do przetworzenia o długości L<sub>i</sub>

Segmenty mogą być zagnieżdźone.
Serwer posiada rejestr wszystkich gier w postaci:
<code>std::unordered_map<unsigned int, ChessGame></code>
gdzie kluczem jest ID gry generowany automatycznie oraz informacje o grze jako wartość. Podczas odłączenia się jednego z graczy, drugi gracz również zostanie rozłączony.

Cała logika serwera znajduje się w klasie <code>TcpServer</code>.
Cała logika klienta znajduje się w klasie <code>TcpSocket</code>, której instancja wysyła sygnały, a te odbierane są przez obiekt klasy <code>ChessBoard</code>.

## Instrukcja
Należy pobrać [archiwum](https://drive.google.com/file/d/1AohSQHM580Jt81AY8rWaQVvBitt8XBQJ/view) i uruchomić. Serwer najlepiej uruchomić z terminala.
Niestety nie mogę zagwarantować działania na wszystkich systemach operacyjnych(projekt był testowany na Ubuntu 20.4), dlatego w razie problemów należy:
- Pobrać [QtCreator](https://www.qt.io/download-open-source?hsCtaTracking=e9c17691-91a0-4616-9bc2-1a6a6c318914%7C963686f8-2c68-442a-b17b-3d73ce95b819)
- Zalogować się na swoje konto
- Zainstalować Qt 5.15
- Pobrać kod serwera i klienta
- Otworzyć obydwa projekty w QtCreator importując plik *.pro
- Uruchomić jeden serwer

## Autor
Przemysław Marcinkowski\
Informatyka stacjonarna, 5 semestr\
Grupa dziekańska IN.6\
Nr albumu 148057\
Email przemyslaw.marcinkowski@student.put.poznan.pl\
Godzina zajęć wtorek 11:45
