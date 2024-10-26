package cz.cvut.fit.tjv.cardatabase.dto;

import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.List;

@Getter
@Setter
public class DealerDTO {
    private Long personal_number;
    private Integer age ;

    private String name ;


    private List<Long> soldCars;


    public DealerDTO(Long personal_number, String name, ArrayList<Long> soldCars, Integer age ) {
        this.personal_number = personal_number;
        this.name = name;
        this.age = age;
        soldCars = new ArrayList<>();
        soldCars.addAll(soldCars);
    }
}
