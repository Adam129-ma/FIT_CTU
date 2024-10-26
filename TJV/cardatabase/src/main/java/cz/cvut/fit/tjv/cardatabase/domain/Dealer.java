package cz.cvut.fit.tjv.cardatabase.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import jakarta.validation.constraints.NotBlank;
import lombok.Getter;
import lombok.Setter;

import java.util.Collection;

@Entity
@Table(name = "dealer")
@Getter
@Setter
public class Dealer implements EntityWithId<Long> {
    @Id
    Long personalNumber;
    @Column(name = "age")
    Integer age;
    @Column(name = "name")
    @NotBlank
    String name;
    @Column(name = "nickname")
    String nickname;
    @ManyToMany
    @JoinTable(
            name = "car_dealer",
            joinColumns = @JoinColumn(name = "personal_number"),
            inverseJoinColumns = @JoinColumn(name = "id_car")
    )
    Collection<Car> soldCars;
    @Override
    public Long getId() {
        return personalNumber;
    }


}
