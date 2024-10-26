package cz.cvut.fit.tjv.cardatabase.service;

import cz.cvut.fit.tjv.cardatabase.domain.EntityWithId;
import org.springframework.data.repository.CrudRepository;
import org.springframework.transaction.annotation.Transactional;

import java.util.Optional;

@Transactional
public abstract class CrudServiceImpl<T extends EntityWithId<ID>, ID> implements CrudService<T, ID> {
    @Override
    public T create(T e) {
        if (e.getId() != null && getRepository().existsById(e.getId()))
            throw new IllegalArgumentException();

        return getRepository().save(e);
    }

    @Override
    public Optional<T> readById(ID id) {
        return getRepository().findById(id);
    }

    @Override
    public Iterable<T> readAll() {
        return getRepository().findAll();
    }

    @Override
    public void update(ID id, T e) {
        if (e.getId() == null || !getRepository().existsById(e.getId()))
            throw new IllegalArgumentException();

        if (getRepository().findById(id).isEmpty())
            throw new RuntimeException(id + " Unavailable");

        getRepository().save(e);
    }

    @Override
    public void deleteById(ID id) {
        if (getRepository().existsById(id))
            getRepository().deleteById(id);
        else
            throw new IllegalArgumentException();
    }

    protected abstract CrudRepository<T, ID> getRepository();

}
