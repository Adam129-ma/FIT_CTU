package cz.cvut.fit.tjv.cardatabase.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import jakarta.validation.constraints.NotBlank;
import lombok.Getter;
import lombok.Setter;

import java.util.Collection;


@Entity
@Table(name = "car")
@Getter
@Setter
public class Car implements EntityWithId<Long> {
    @Id
    @Column(name = "id_car")
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;
    @Column(name = "production_year")
    @NotBlank
    private Integer productionYear;
    @Column(name = "colour")
    @NotBlank
    private String colour;
    @Column(name = "brand")
    @NotBlank
    private String brand;

    @ManyToMany(mappedBy = "soldCars")
    @JsonIgnore
    private Collection<Dealer> soldBy;
    @OneToMany(mappedBy = "boughtCar")
    private Collection<Customer> boughtBy;
    @Override
    public Long getId() {
        return id;
    }
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Car c) {
            return id == null ? id == c.id : id.equals(c.id);
        } else
            return false;
    }

    @Override
    public int hashCode() {
        return id == null ? 0 : id.hashCode();
    }

}