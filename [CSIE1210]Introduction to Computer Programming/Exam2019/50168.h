int stationDistance(int R_distance[],int G_distance[],int B_distance[],int G_origin,int G_destination,int startStation[],int endStation[]){
    int distance = 0;
    if(startStation[0] == 1 && endStation[0] == 0){
        startStation[0] = 0;
        endStation[0] = 1;
        int temp = endStation[1];
        endStation[1] = startStation[1];
        startStation[1] = temp;
    }
    else if(startStation[0] == 2 && endStation[0] == 0){
        startStation[0] = 0;
        endStation[0] = 2;
        int temp = endStation[1];
        endStation[1] = startStation[1];
        startStation[1] = temp;
    }
    else if(startStation[0] == 2 && endStation[0] == 1){
        startStation[0] = 1;
        endStation[0] = 2;
        int temp = endStation[1];
        endStation[1] = startStation[1];
        startStation[1] = temp;
    }
    if(startStation[0] == 0 && endStation[0] == 0){
        if(startStation[1] > endStation[1]){
            for(int i = endStation[1]; i < startStation[1]; ++i)
                distance += R_distance[i-1];
        }
        else{
            for(int i = startStation[1]; i < endStation[1]; ++i)
                distance += R_distance[i-1];
        }
        return distance;
    }
    else if(startStation[0] == 0 && endStation[0] == 1){
        if(G_origin < startStation[1]){
            for(int i = G_origin; i < startStation[1]; ++i)
                distance += R_distance[i-1];
        }
        else if(G_origin > startStation[1]){
            for(int i = startStation[1]; i < G_origin; ++i)
                distance += R_distance[i-1];
        }
        for(int i = 1; i < endStation[1]; ++i)
            distance += G_distance[i-1];
        return distance;
    }
    else if(startStation[0] == 0 && endStation[0] == 2){
        int i = 0;
        while(G_distance[i]){
            distance += G_distance[i];
            ++i;
        }
        if(startStation[1] < G_origin){
            for(i = startStation[1]; i < G_origin; ++i)
                distance += R_distance[i-1];
        }
        else{
            for(i = G_origin; i < startStation[1]; ++i)
                distance += R_distance[i-1];
        }
        if(endStation[1] < G_destination){
            for(i = endStation[1]; i < G_destination; ++i)
                distance += B_distance[i-1];
        }
        else{
            for(i = G_destination; i < endStation[1]; ++i)
                distance += B_distance[i-1];
        }
        return distance;
    }
    else if(startStation[0] == 1 && endStation[0] == 1){
        if(startStation[1] < endStation[1]){
            for(int i = startStation[1]; i < endStation[1]; ++i)
                distance += G_distance[i-1];
        }
        else{
            for(int i = endStation[1]; i < startStation[1]; ++i)
                distance += G_distance[i-1];
        }
        return distance;
    }
    else if(startStation[0] == 1 && endStation[0] == 2){
        int i = startStation[1];
        while(G_distance[i-1]){
            distance += G_distance[i-1];
            ++i;
        }
        if(endStation[1] > G_destination){
            for(i = G_destination; i < endStation[1]; ++i)
                distance += B_distance[i-1];
        }
        else{
            for(i = endStation[1]; i < G_destination; ++i)
                distance += B_distance[i-1];
        }
        return distance;
    }
    else if(startStation[0] == 2 && endStation[0] == 2){
        if(startStation[1] < endStation[1]){
            for(int i = startStation[1]; i < endStation[1]; ++i)
                distance += B_distance[i-1];
        }
        else{
            for(int i = endStation[1]; i < startStation[1]; ++i)
                distance += B_distance[i-1];
        }
        return distance;
    }
    return 0;
}