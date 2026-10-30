#pragma once

namespace dino
{

// Forward declaration
class Player;

class ILassoTarget
{
public:
    virtual ~ILassoTarget() = default;

    /// Déclenché par le système de lasso lorsque l'objet se trouve dans une boucle fermée
    virtual void OnCaughtInLasso(Player* pLassoOwner) = 0;
};

} // namespace dino