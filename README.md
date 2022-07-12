# The Snake Game for Group 15 
# CS1605
*Team member: Minghao Liu; Siyuan Qi; Shixiang Song*\
组员: 刘洺皓, 齐思远, 宋士祥
 
Contact us: 联系我们\
lmh209@sjtu.edu.cn  qisiyuan7936@sjtu.edu.cn songshixiang@sjtu.edu.cn
## Introduction for the game 游戏简介
This is an assigment for course CS1605 in Shanghai Jiao Tong Uninersity. \
本作品系上海交通大学程序设计实践课小组大作业\
The snake game is firstly put out in Gremlin platform in 1976. Thanks for the popularization from Nokia, it has become one of the most famous game in the world, and people has developed a lot of versions vehind them. In 2016, a Wechat miniprogram called *the SnakeWar*(贪吃蛇大作战) made this game popular again.
 
Our team also develops a snakegame with the C++ and ncurses library. This game not only includes the basic function of the snake game, but also includes our own thoughts.
## Preview of our game. 游戏速览
![](https://notes.sjtu.edu.cn/uploads/upload_8cd5ccc079e54d1a1340aa869fb4798d.png)
![](https://notes.sjtu.edu.cn/uploads/upload_745e807547832a1005c3ec923aee43b6.png)
![](https://notes.sjtu.edu.cn/uploads/upload_346aaa3d87798c2ce81b8df5ae349494.png)
![](https://notes.sjtu.edu.cn/uploads/upload_e1d201986e419b7cb8e499d27b23a669.png)
![](https://notes.sjtu.edu.cn/uploads/upload_aa06f69b4b1bd6940ff6ca3c59fe03df.png)
 
## Features 游戏特性
 
*   Provides more APIs to manipulate the snake and to modify relevant features.
*   Actualizes the Double mode where two opponents maneuver their own snakes in their own windows\.
*   Furnishes an API for adjusting the countdown under the limits of which players are able to compete\.
*   Implements the machine snake whose smartness is dynamically modified by the moving speed and the fixed-in difficulty\.
*   Copious terrains implemented to make the movement of the snake colorful\.
*   Exciting survial mode and wonderful mechanisms for the snake to embark on a journey filled with both pitfalls and rewards\.
*   Crucial mode leading the users into an tense, intricate plot with different equipments and skills involving revivecoin, HP, BOSS and so on. It enables the user to plunge into a overwhelming adventure to face against the mighty aliens and bring glory back to our motherland\.
 
## The symbol
@ for the snake body.
#/◆ for the food
 
## Mode
 
### Solo
*   *Difficulty*: the emblem of the velocity of the snake. The velocity of the snake skyrockets in exponential frame as the difficulty rises up. The underlying mechanism denotes that the velocity of the snake is $\dfrac{1}{0.75^{\mathrm{difficulty}}}$ units per milliesecond.
*   *Has dynamic difficulty*: the API provided for users to control whether the velocity will be dynamically modified when our snake is on its wonderful journey. When it is on, the velocity of the speed will ascend (the Difficulty will rise by one) whenever the player gain five more points.
*   *Has walls*: the API furnished for users to decide whether the field has walls along the process. 
 
*	
### Double
 
*   *Speed*: Velocity of the snake. The velocity of the snake skyrockets in exponential frame as the difficulty rises up. The underlying mechanism denotes that the velocity of the snake is $\dfrac{1}{0.8^{\mathrm{difficulty}}}$ units per milliesecond \(The base is modified form 0.75 to 0.8 as the player enjoys the game in a smaller frame of window\).
*   *Countdown*: The countdown under which two players are to compete. When altering the countdown, the cursor will be immediately trailing the user's input. The Backspace key will enable the user to revert to the previous digit. If feeling uncomfortable with the current change, feel free to touch the first digit to revise the countdown again. \(Notice: A warning window will pop up when you inputs a number invalid\. A valid countdown is deemed to be a time denoting in the format of 'minutes:seconds' anywhere between 0:00 to 9:59\.)
*   *Has walls*: it works the same with the one in the Solo mode.
*   *Man to ?*: The opponent type. You will be confronting you peer under "**Man to Man**" mode. However, you will be confronting a machine snake whose wisdom and smartness tops the pinnacle of human's intelligence when under the "**Man to Computer**" mode.
*   *Difficulty*: this item will only show up if you are under "**Man to Computer Mode**". Categorized into *"Easy"*, *"Normal"*, *"Medium"*, *"Hard"*, the difficulty is the epitome of the smartness of the machine snake. Tricky and prudent as the machine snake is, this Difficulty may still save you from being smashed by the computer. The underlying mechanism is to use the theory of probability and four indexs to compose the overall intelligence level of the machine snake. \(Mistackle: the indication of the snake's probability of missing the food. Strike: the embodiment of the snaks's capability of hitting the food in short range. MoveBack: the abstraction of the snake's ability of the sharpness of sensing the food. Collision: the conceptualization of the snake's ability to avoid the collision. The machine snake operates on these four indexes which are dynamically altered in accordance with the Difficulty the uses has chosen and the velocity of the two snakes.\)\.
 
### Survival Mode
This mode is for survive mode. In this mode, we have different kinds of food: 
- **\*** for randomly increase or decrease difficulties(with $30\%$ possiblity to increase difficulty and $70\%$ to decrease.)
- **%** for the switch of special terrian(with $30\%$ possiblity to add a terrian and $70\%$ to remove a terrian.)
- **\$** It may double your gain points or may make your next five food cannot get your points.
- **?** Make your snake become invincible or confusion.
 
There are also some special terrians in this mode.
- **H** for the mountians. The game is over once get hit.
- **~** for the rivers. The snake will be invincible for five seconds.
- **h** for hills. The snake will decrease its length.
 
There are two special states to interpret.
- *Invincible*: Increase the velocity and won't be dead even hit itself.
- *Confusion*: Reverse the direction.
 
\* % $ ?都是特殊食物
\* 百分之三十增加1难度 百分之70减少1难度
% 百分之30增加一个特殊地形 百分之70减少一个特殊地形
$ 百分之40接下来五个食物得分加倍 百分之30接下来五个不得分 百分之30赋予吸铁石效果,即头部7*7范围即视为吃到食物(仅限普通食物)
? 百分之60进入无敌状态10S,百分之40进入迷惑状态15S
 
\~ H h {}都是特殊地形
\~ 河水,经过后赋予5S无敌时间
H 高山,撞上立即死亡
h 小山,撞上蛇长度减1,得分减1,小山被撞碎消失
{} 分别只会在左右边界出现,代表可以穿墙
 
关于状态(三种)
普通状态(Normal)
无敌状态(Invincible):移动速度大幅加快,不会死亡(可以撞到自己,遇到边界自动翻墙,遇到高山不会死亡)
迷惑状态(Confusion):上下左右键盘效果颠倒
贪吃蛇只会出现三种状态中的一种,如果本身处于某种状态,又获得新状态,则原状态作废,变更为新状态
 
游戏难度:由总得分以及特殊食物'*'共同影响,其计算为总得分对5取整加上*产生的难度,难度越大,蛇的移动速度越快
地图机制:普通食物已经介绍;
        特殊食物每隔五秒随机出现一个,场上最多存在三个,而且会每隔10S检测一次食物数量是否达到3,如果检测时间食物数量是3,会更新最早的那个食物
                特殊地形每隔10S随机出现一个,场上最多存在(2*难度+1)个,而且会每隔15S检测一次地形数量是否达到最大数量,如果是,会更新最早生成的那个地形
 
 
### Crucial Mode
Background: In 2022, the alien invade the earth. And our snake warrior are trying to protect our mother land.
 
#### Stage 1
Running to the enemies' base. Please survive within the time limit.
 
Some situation for the game.
 
| Symbol  | Name  | Function            | Time for appearing  | Numbers |
| ------- | ----- | ------------------- | ------------------- | ------- |
| *       | Bomb  | Decrease the length | 5s                  | 3       |
| ------- | Laser | End the game        | 10s, 3s for lasting | 1       |
| #       | Food  | Increase the length | 5s                  | 1       |
 
You have five chances. The time limitation is 100s.
 
#### Stage 2
You are sent to fight angainst the boss.
 
You will get different equipent to help you pass this stage.
- **Weapon**: Increase the damage to the enemy. Make the boss dizzy for five seconds on each hit. The snake will make laser each 10s and will last for 3s .
- **Shield**: The snake will be invincible for 3s in each 10s. The damage to the soider will be double. Decrase the damage from the enemy.
- **Ornaments**: It may be more possible to make the critical hit. The snake will not be dizzy. The damage to the boss will double in damaging period.
 
The Rules:
1. The boss sitting in the center of the screen, with the HP of 500. When the HP is 400/300/200/100, the boss cannot be hurt. If the snake cannot solve this problem, the game is over. After this stage, the boss can move and the damage will double.
2. The damage from the boss
* Randomly generlize 20 bombs. When hit, the HP of the snake may decrease 2.
* Generlize 8 bullets(-). When hit, the HP of the snake may decrease 2.
* Randomly generlize marshes(\~\~\~). The velocity may decrese once touched.
* Randomly generlize 3 soilders(\%) The HP will decrease 2 when hit. 
 
10s for a loop.
 
3. The special stage for the "hundred maze". To 
- The screen will show a string. Type this string in ten seconds.
- Genearlize a rail. Make the snake through the rail
- Push the four $ near the boss.
- make the \[ \] symbol the the same stage.
 
4. The attack method.
    When the snake's length is more than 5, the snake can strike the snake. The length of the snake will decrease 5, meanwhile the HP of the boss may decrease 5.
     
2222年,外星人入侵地球,我们的贪吃蛇挺身而出,踏上征程,下定决心消灭外星人,保卫地球
 
攻坚模式分为两个阶段
 
阶段1(跑酷模式):
贪吃蛇奔赴外星人老巢,请帮助他顺利到达
贪吃蛇初始长度是6（五点生命值）,时间归零前生命值大于等于1即可胜利
 
地图机制:
1、每间隔5S地图会随机生成3个炸弹（*），碰撞则生命值减少1
2、每间隔10S，地图右侧会发出外星人的激光（-----------------），贪吃蛇触碰即会死亡，持续3S，发射激光前，这一行右侧会出现！(2S)作为警告
3、每间隔5S，地图会出现一个食物（#) ,吃后生命值+1
4、攻坚模式开始时贪吃蛇获得五个复活币,死亡后可选择是否复活
5、一阶段原定时间为100S
 
 
阶段2：
1、准备阶段，贪吃蛇在如下装备中各随机获得一种，装备列表如下
武器：A:贪吃蛇伤害加倍       
B:每次攻击后，赋予BOSS眩晕效果，暂停行动5S   
C:每间隔十秒贪吃蛇会向前持续发射激光3S,激光(+)触碰BOSS即可持续造成伤害
 
防具：A：每隔10S赋予蛇无敌状态3S,无敌状态移动速度加快,不会被伤害,且可以穿墙
B：击杀外星人小兵后体力加1   
C：敌人所有伤害减半    
 
首饰：（1）A:暴击率提高至百分之50,暴击时伤害加倍        
B:贪吃蛇不会被眩晕异常效果影响   
C:BOSS破防时期,伤害增加百分之五十(只针对冲撞攻击)
 
2、游戏机制
（1）、BOSS处于屏幕中间的王座上，初始血量500；血量下降至400/300/200/100时，进入不可伤害状态，此时破解机制，可令BOSS虚弱一段时间,虚弱时BOSS不会发起进攻;若没有破解,贪吃蛇则会死亡;
机制4结束后,BOSS进入狂暴状态,会不断移动,且伤害加倍.
（2）、BOSS常驻攻击方式：
A 、在地图上随机生成20个炸弹，显示！作为警告，两秒后生成炸弹*，炸弹存在3S后消失，如碰到炸弹生命值减少2；
B、以BOSS为中心，向周围八个方向发射弹药攻击（-），触碰则生命值-2
C、随机生成五个3*3的沼泽（~~~），陷入则移动速度减慢5S；
D、随机召唤3个小兵（%），触碰则生命值减少2.
 
这四个攻击方式会每隔10S进行一次
进入狂暴状态后，各种攻击会加强
 
（3）每当BOSS血量减少100，会出现一次特殊机制，破解后，BOSS进入虚弱状态20S，否则，贪吃蛇死亡，BOSS不虚弱
机制1：屏幕会出现一串字符，在10S内打出这些字符方可成功
机制2：地图生成~模样的轨道，全部通过轨道方可破防,若走错,则会返回原点重新开始
机制3：地图生成4各$标志，将其推到BOSS处，方可破防
机制4:地图随机生成十个[ ]标志,触碰则会变换为另外一种,场上20个都变一样则破解
 
（4）贪吃蛇的攻击方式：通过吃食物，不断长大，长度大于等于5后，可对BOSS冲撞，造成5点伤害，并牺牲这部分身体
 
 
## Terrains
*   *Plain*: boundless and flat plain for our snake to embark on its adventure\. No
 effects\.
*   *Water*: Shiny waves and broad blue vista\. Unfortunately, our snake hasn't been managed to learn to swim since its childhood. Once the snake's head is in the water, it is drowning desperately and the progress bar in the upper window will be indicating the drowning countdown. And if it has been drowning for more than five seconds, our snake will be gone forever...
*   *Mountain*: Steep hills and hard rocks/. Once the head hits the mountain. Our wretched snake will have to say farewell to its wonderful journey.
*   *Forest*: Dense trees and large marshes. Once the snake's head writhes into this dense forest, the velocity will decrease by half\.
     
 
 
 
## Installition 安装
1. System version 系统要求\
Linux:  [Ubuntu 22.04](https://releases.ubuntu.com/22.04/ubuntu-22.04-desktop-amd64.iso.torrent?_ga=2.216621590.160370100.1657544528-639798126.1647262586  )\
Windows: please use WSL  请使用wsl进行安装
* input ```wsl --install``` in cmd(administor). 在cmd(管理员)中输入```wsl --install``` 
* input ```wsl --install d Ubuntu``` in cmd. 在cmd(管理员)中输入```wsl --install d Ubuntu```
2. Library requirement 库的安装\
Input the following commands in the terminal. 在终端输入以下指令:
- ```sudo apt-get install g++``` intall/安装g++
- ```sudo apt-get install libncurses-dev``` install ncurses library/ 安装ncurses库
3. Clone the file 克隆仓库\
```git clone https://gitee.com/lmh209/snakegame```
4. snakegame input under the file 目录下输入 ```make```
* If you fail to make, add ```-lpthread``` at the end of the line if this line contians ```g++```
* 如果提示构建错误, 请在含有```g++```的代码行后方添加```-lpthread```
5. input 输入 ```./snakegame``` and start the game 并开始游戏
 
## Key-Operation Description 按键说明
* Control the snake:
  For solo player, the **WASD** and ⬆⬅⬇➡ are both available to control the snake. For double players, we recommend *Player A* to use **WASD** as contolling  method.
* 蛇的控制:
  对于单人玩家, 使用WASD(**请提前关闭中文输入法!**)和上下左右键均可控制蛇
* Pulse the game:
  Please enter **Backspace** or **SPACE** when you try to pulse the game(for solo and double modes only). It is also avalable when you try to move to the parent menu.
* 暂停游戏:
  按后退键和空格键均可实现游戏暂停(仅在单人和双人模式有效). 返回上级菜单也可采用相同方式
   
## Menu 菜单
The *Mode* menu is to choose the game mode and the terrians.\
*Mode* 菜单用于设置游戏模式和地形\
You could use **Enter** or your mouth to control it.\
您可以直接使用鼠标或回车键进行设定\
The *Setting* menu is for setting the difficulty.
*Setting*按钮用于设置难度
 
## Barriers yet encountered 目前开发中的问题
*   *Machine Snake*: Cannot come up with a perfect algorithm for the machine snake to avoid trapping itself into a pitfall under the traditinal frame of "if else" and "while" clauses\. The fundamental checks other than hitting itself, wall and the mountain are the circuit check in which the snake is required to take measures before forming a circuit which could be the premonition of a crash\. We are trying to use the neural network to update this algorithim
*   *Multi-thread*: When in the *Double* mode, two players should be playing in their seperate boundaries, and this can be preferably implemented using ```std::thread```. Executing two threads dictated by two lambda functions, each of which is used to maneuver its own snake and render its relevant information and even to manipulate the snake at different rates\. But because the ncurses environment operates on static variables to execute the render action, multi-threads can only operate stably when only one channel under which occasion other threads are to be independent of the GUI library and finish the underlying computing procedures.
*   机器蛇的逻辑设置不完全合理, 拟采用神经网路对算法进行更新.
*   由于GUI逻辑设置问题, 在多人游戏时存在显示bug
## The future game with GUI 可视化实现(未来升级版)
This is a version unfinished. We use the QT6 library to implement the GUI verison.\
一个尚未完成开发的版本. 该版本采用QT6库进行重构.\
Since the time limit, some important functions like controlling the volume has not been developed yet. But with the background music and the GUI interface, we are looking forward to your entertainment in the game.\
Some images(Not finished yet!)
![](https://notes.sjtu.edu.cn/uploads/upload_281b4176040d5cea482adafb7830ccc3.png)
 
![](https://notes.sjtu.edu.cn/uploads/upload_2b03950f0281eb33cd04be5465ee35a6.png)
