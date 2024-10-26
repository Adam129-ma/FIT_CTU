package cz.cvut.fit.tjv.cardatabase.dto;


import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.List;
@Getter
@Setter
public class CarDTO{
    private Long id;

    private Integer production_year;

    private String colour;
    private String brand;

    private List<Long> customerIds;

    public CarDTO(Long id,  Integer production_year, String colour, String brand, CustomerDTO[] customers) {
        this.id = id;
        this.production_year = production_year;
        this.colour = colour;
        this.brand = brand;
        customerIds = new ArrayList<>();

        for ( var customer : customers )
            customerIds.add(customer.getId());
    }

}

