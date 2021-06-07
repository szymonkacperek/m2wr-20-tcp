#ifndef MAIN_H
#define MAIN_H

#include <math.h>
#include <ncurses.h>

typedef struct{
    double v;
    double w;
}AppDataVelocities;

typedef struct{
    int i_ver;
    int i_content_id;
    union{
        AppDataVelocities velocity;
        unsigned char cTab[64];
    }AppData;
    /* AppData data; */
}TPacket;

class RobotModel{
    public:
        RobotModel(double new_x_,  double new_y_, double new_theta_){
            x_ = new_x_; y_ = new_y_; theta_ = new_theta_;
        }

        void Update(double v, double omega){
            x_ += dt_*v*cos(theta_);
            y_ += dt_*v*sin(theta_);
            theta_ += dt_*omega;
        }

        void GetState(double *p_x, double *p_y, double *p_theta, double *p_v, double *p_w){
            *p_x = x_;
            *p_y = y_;
            *p_theta = theta_;
            *p_v = y_/(sin(theta_));
            *p_w = theta_;
        }

    private:
        double x_, y_, theta_;
        double dt_ = 0.02;
};



#endif
