# Microprocessor-control-system-with-visual-feedback-for-a-sorting-robot
Control program for a 6-axis robot using vision feedback, real-time libraries and with the possibility of manual control.

ENG

The idea for this project, which is also my engineering thesis, was to test Cognex cameras as position feedback. These cameras are often used in other applications such as reading qr or barcodes. Using already installed cameras as additional position feedback would help to eliminate many random errors during production and as a result save money with no investment or hardware change. For the tests to be reliable, it was necessary to create an environment similar to that in the factory. The most popular solution was therefore chosen, namely a robot that manipulates the product on the production line.


The robot was built using 3D printing. The design of the robot that was used is open source. Minor changes were made to it to suit the chosen application. For example, a motor was added in one of the axes to increase the possible load on the arm, and the cover of the control part was modelled to suit the selected components.

The main tasks were:
- build the robot and control system based on stm32
- develop the control of a 6-axis robot with simple and inverse kinematics using interrupts as well as real-time libraries (freeRTOS)
- developing an android application to control the robot
- building and programming a production line model based on the S7-1500 controller using a Cognex camera
- implementation of PROFINET/ETHERNET communication between camera, PLC and stm32 using LwIP library
- test the positioning capabilities of the Cognex camera.

3D design of the robot: https://hackaday.io/project/12989-thors

PL

Pomysłem na ten projekt, będący jednocześnie moją pracą inżynierską było przetestowanie kamer Cognex jako sprzężenie zwrotne od pozycji. Kamery te stosowane są często w innych zastosowaniach np. przy sczytywaniu kodów qr lub kreskowych. Wykorzystanie już zainstalowanych kamer jako dodatkowe sprzężenie od pozycji pomogłoby zniwelować wiele błędów losowych podczas produkcji a w rezultacie zaoszczędzić pieniądze praktycznie bez żadnej inwestycji ani zmiany sprzętowej. Aby testy były wiarygone należało stworzyć środowisko podobne do tego w fabryce. Wybrano więc najpopularniejsze rozwiązanie, czyli robota manipulującego produktem na linii produkcyjnej.


Robot został zbudowany z wykorzystaniem druku 3D. Projekt konstrukcji robota, który wykorzystano jest projektem open source. Wprowadzono  w nim niewielkie zmiany, aby dostosować go do wybranego zastosowania. Dodano np. silnik w jednej z osi aby zwiększyć możliwe obciążenie ramienia oraz zamodelowano obudowę części sterującej dostosowaną do wybranych komponentów.

Głównymi zadaniami było:
- zbudowanie robota oraz autorskiego układu sterowani opartego o stm32
- stworzenie sterowania robota 6 osiowego wraz z kinematyką prostą, odwrotną z wykorzystaniem przerwań, jak i bibliotek czasu rzeczywistego (freeRTOS)
- stworzenie aplikacji na system android do sterowania robotem
- zbudowanie i zaprogramowanie modelu linii produkcyjnej opartej na sterowniku S7-1500 z wykorzystaniem kamery Cognex
- zaimplementowanie komunikacji PROFINET/ETHERNET między kamerą, PLC a stm32 z wykorzystaniem biblioteki LwIP
- przetestowanie możliwości kamery Cognex w pozycjonowaniu.

Projekt 3D robota: https://hackaday.io/project/12989-thors
