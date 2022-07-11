#ifndef BATTLE_H
#define BATTLE_H

#include "game.h"

class Battle:public Game{
public:
    Battle();
    ~Battle();

    void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;

    void createInstructionBoard();
    void renderInstructionBoard() const;

    void renderBoards() const;

    void initializeGame();
    void runGame_main();         //����ģʽ������Ϸ
    void runGame_run();       //�ܿ�ģʽ������Ϸ
    void runGame_crucial();      //����ģʽ������Ϸ
    void renderPoints() const override;
    void renderDifficulty() const override;

    void createRandomFood();
    void createRandomSpecialFood();          //����ģʽ�������ʳ��
    void createRandomSpecialTerrain();        //����ģʽ�����������
    void createRandomFood_run();             //�ܿ�ģʽ�����������
    void createRandomBomb();              //�ܿ�ģʽ�������ը��
    void createRandomLight();             //�ܿ�ģʽ������ɼ���
    void createWarning();                 //�ܿ�ģʽ���漴�����ɵļ���
    void controlSnake();  //const;               //̰��������ʱ�������ж�
    void controlSnake_confusion(); //const;     //̰����ѣ��ʱ�����ж�
    void controlSnake_run();                    //�ܿ�ģʽ����̰�����ж�

    Status runGame();
    bool renderRestartMenu() const;
    void adjustDelay();
    void adjustScene();                       //����ģʽ��������
    void adjustSceneRun();                     //�ܿ�ģʽ��������
    void adjustSceneCrucial();                 //����ģʽ��������
    void moveScene();                         //�ܿ�ģʽģ�ⳡ����̬�仯

    void renderFood() const;
    void renderSnake() const;
    void selectModes();                      //ѡ��ģʽ
    void renderSFood();
    void renderSTerrain();
    void renderWarning();                      //��Ⱦ����
    void renderLight();                       //��Ⱦ����
    void renderLives();                      //��Ⱦ̰��������ֵ
    void renderRevivecoins();               //��Ⱦ���������
    bool renderResult();                    //�ܿ�/������Ⱦ��Ϸ���
    void checkSFood();                       //���Ե�������ʳ�ﲢ������ӦЧ��
    void checkSTerrain();                     //���������β�������ӦЧ��
    void checkLight();                      //����Ƿ񱻼������(�ܿ�ģʽ)
    void rand_operation();                 //������ʳ��?�������Ч��
    void delRandSTerrain();                //���ɾ��һ������
    bool checkDeath();                     //���̰�����Ƿ�����(�ܿ�/����)
    bool weatherRevive();                  //��Ҿ����Ƿ񸴻�

    void bossAttack();                  //boss��������
    void renderboss();                //��ȾBOSS
    void moveboss();                 //����ƶ�BOSS
    void way1();               //BOSS������ʽ1
    void way2();          //BOSS������ʽ2
    void way3();          //BOSS������ʽ3
    void way4();          //BOSS������ʽ4
    void renderbomb();    //��Ⱦը��
    void renderammu();    //��Ⱦ�ӵ�
    void renderswamp();     //��Ⱦ����
    void rendersoldier();     //��ȾС��
    void attack();         //̰���߶�BOSS��������
    void lightAttack();      //̰���߼��⹥��
    void specialMechanism1();     //�������1
    void specialMechanism2();      //�������2

    void specialMechanism3();       //�������3

    void specialMechanism4();    //�������4

    void createOrbit();        //����������
    void createBox();       //�����������
    void control_mach2();      //���ڻ���2ʱ����̰����
    void renderOrbit() const;    //��Ⱦ���
    void renderBox() const;     //��Ⱦ����
    bool checkBox();          //����Ƿ��Ƶ�����
    void createDoor();           //���������
    void renderDoor() const;         //��Ⱦ��
    void changeDoor();            //�ı��ŷ���
    bool checkDoor();            //������Ƿ�Ϊͬһ����

    void changeMode(int ); //change Mode



private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    //int mScreenWidth;
    //int mScreenHeight;
    //int mGameBoardWidth;
    //int mGameBoardHeight;
    int mInformationHeight = 6;
    int mInstructionWidth = 18;
    // Snake information
    //const int mInitialSnakeLength = 2;
    //const char mSnakeSymbol = '@';
    //std::unique_ptr<Snake> mPtrSnake;
    // Food information
    //SnakeBody mFood;

    std::vector<std::pair<char,SnakeBody>> sFood;
    std::vector<std::pair<char,SnakeBody>> sTerrain;
    std::vector<SnakeBody> sFood_run;
    std::vector<std::pair<char,SnakeBody>> sTerrain_run;

    const char mFoodSymbol = '#';        //   ��ͨʳ��
    //int mPoints = 0;
    //int mDifficulty = 0;
    int mDifficulty_Extra = 0;           //����ģʽ����ʳ��'*'����ĸ����Ѷ�
    int mBaseDelay = 200;
    int mDelay=200;
    int tempDelay;     //���Ǹ�ʲô����,����ûʲô��
    int lives;        //�ܿ�/����ģʽ����ֵ
    const std::string mRecordBoardFilePath = "record.dat";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
    int mode = 1;          //��Ϸģʽ
    int time_begin;    //������Ϸ��ʼʱ��
    int time_finish;   //���й���ѭ��һ�ν���ʱ��
    int time_temp;     //���й���ѭ��һ�ο�ʼʱ��
    int time1;   //��������ʳ���ʱ��(����) ����BOSS����ʱ��(����)
    int time2;   //����������ε�ʱ��(����) ����ը��ʱ��(�ܿ�) �����Ƿ�boss�����ƶ�
    int time4;   //���Ƽ���ʱ��(�ܿ�)  ���߷���A�޵�Ч��(����)
    int pos_light;    //�ܿ�ģʽ��������λ��
    int check_double=0;      //�������������ʳ��˫���÷�(����)
    int check_zero=0;        //�������������ʳ�ﲻ�÷�(����)
    int time_magnet;         //��������ʯʱ��
    bool confusion=0;        //�ж�̰�����Ƿ�ѣ��
    bool invincible=0;       //�ж�̰�����Ƿ��޵�
    int time_confusion=0;    //ѣ��ʱ��
    int time_invincible=0;   //�޵�ʱ��
    int time_all_run;        //�ܿ�ģʽʣ��ʱ��
    int time_warning;        //���漤��(�ܿ�)����ը��(����)��ʱ��
    int time_light;          //����ʣ��ʱ��(�ܿ�)
    int reviveCoins;         //ʣ�ิ�������(�ܿ�/����)
    bool victory=true;       //�ж��Ƿ�ʤ��(�ܿ�/����)

//���¾�Ϊ����ģʽ�õ��Ĳ���
    int livesBOSS;          //BOSSʣ��Ѫ��
    int conditionBOSS;      //״̬1Ϊ��ͨ,״̬3Ϊ�����˺�,״̬4Ϊ����
    int harm;               //̰���߶�BOSS���˺�
    int critChance;         //������
    int harmBOSS;           //BOSS�Ĺ�����̰������ɵ��˺�
    std::string weapon;     //����
    std::string armor;      //����
    std::string jewelry;    //����
    int attackBossWay;       //BOSS��ͨ�����ķ�ʽ
    SnakeBody boss;          //BOSS��λ��
    std::vector<SnakeBody> bomb;    //�洢ը��λ��
    std::vector<std::pair<SnakeBody,SnakeBody>> ammunition;       //�洢BOSS�ӵ�λ��
    std::vector<SnakeBody> swamp;       //�洢����λ��
    std::vector<SnakeBody> soldier;     //�洢С��λ��
    int time_bomb;          //ը������ʱ��
    int time_ammuition;    //BOSS�ӵ�����ʱ��
    int ammu;              //�����ӵ�λ�õĲ���
    int time_swamp;        //�������ʱ��
    int time_lowspeed;     //����debuffʱ��
    int reserve;           //����Ϊ"C"ʱ,��������ʱ��
    std::vector<SnakeBody> light;      //����ΪCʱ,�洢̰���߷��������λ��

    int checkCrit;         //�����ж��Ƿ񱩻�
    int time_temp1;       //ĳ�������õ�,�ǲ�����
    int time_temp2;       //ͬ��
    int time_special_attack;       //Ҳ�ǲ�����
    int mechanism;    //��һ������Ϊ��ȷ˳��;�ڶ���Ϊ���ض����;������Ϊ����Ʒ�Ƶ�BOSS��;���ĸ�Ϊʹ��ͼ��ͼ����ͬ;
    bool if_crack;        //�жϻ���1�Ƿ��ƽ�
    bool if_crack2;       //ͬ��
    bool if_crack3;       //ͬ��
    bool if_crack4;       //ͬ��
    bool if_worked;       //�жϻ���1�Ƿ񴥷�
    bool if_worked2;      //ͬ��
    bool if_worked3;     //ͬ��
    bool if_worked4;      //ͬ��
    int time_weak;        //BOSS����ʱ��
    char print_char;      //����1Ҫ������ַ�
    int time_mechanism;    //����1ʣ��ʱ��
    int time_mechanism2;   //ͬ��
    int time_mechanism3;     //ͬ��
    int time_mechanism4;       //ͬ��
    int checkConfusion;       //̰���ߵ�����ֵ
    std::vector<std::pair<char,SnakeBody>> orbit;      //����2���ɵĹ��
    std::vector<SnakeBody> Box;                    //����3���ɵ�����
    std::vector<SnakeBody> DoorLeft;                //����4���ɵ���
    std::vector<SnakeBody> DoorRight;              //����4���ɵ���
};

#endif //BATTLE_H
