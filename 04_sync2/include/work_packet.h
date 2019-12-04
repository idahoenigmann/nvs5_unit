//
// Created by ida on 04.12.19.
//

#ifndef HOENIGMANN_I15013_WORK_PACKET_H
#define HOENIGMANN_I15013_WORK_PACKET_H

class WorkPacket {
public:
    WorkPacket(int id) {
        this->id = id;
    }

    int get_id() {
        return id;
    }

private:
    int id;
};

#endif //HOENIGMANN_I15013_WORK_PACKET_H
