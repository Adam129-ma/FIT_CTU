package cz.cvut.fit.tjv.cardatabase.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import jakarta.validation.constraints.NotBlank;
import lombok.Getter;
import lombok.Setter;


@Entity
@Table(name = "customer")
@Getter
@Setter
public class Customer implements EntityWithId<Long> {
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    @Column(name = "id_customer")
    Long id;
    @Column(name = "name")
    String name;
    @ManyToOne
    @JsonIgnore
    @JoinColumn(name = "bought_car")
    Car boughtCar;
    @Override
    public Long getId() {
        return id;
    }
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Customer c) {
            return this.id.equals(c.id);
        } else
            return false;
    }

}