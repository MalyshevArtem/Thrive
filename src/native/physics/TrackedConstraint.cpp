// ------------------------------------ //
#include "TrackedConstraint.hpp"

#include "Jolt/Physics/Constraints/Constraint.h"

#include "PhysicsBody.hpp"

// ------------------------------------ //
namespace Thrive::Physics
{

TrackedConstraint::TrackedConstraint(const JPH::Ref<JPH::Constraint>& constraint, const Ref<PhysicsBody>& body1) :
    firstBody(body1), constraintInstance(constraint)
{
    if (constraint == nullptr || body1 == nullptr)
        throw std::runtime_error("missing constraint or body for tracked constraint");

    firstBody->NotifyConstraintAdded(*this);
}

TrackedConstraint::TrackedConstraint(
    const JPH::Ref<JPH::Constraint>& constraint, const Ref<PhysicsBody>& body1, const Ref<PhysicsBody>& body2) :
    firstBody(body1),
    optionalSecondBody(body2), constraintInstance(constraint)
{
    if (constraint == nullptr || body1 == nullptr || body2 == nullptr)
        throw std::runtime_error("missing constraint or body for tracked constraint");

    firstBody->NotifyConstraintAdded(*this);
    optionalSecondBody->NotifyConstraintAdded(*this);
}

TrackedConstraint::~TrackedConstraint()
{
    if (IsAttachedToBodies())
    {
        firstBody->NotifyConstraintRemoved(*this);

        if (optionalSecondBody != nullptr)
        {
            optionalSecondBody->NotifyConstraintRemoved(*this);
        }
    }

    if (createdInWorld != nullptr)
        LOG_ERROR("Constraint on destruction still exists in a world, this will likely crash the physics system");
}

void TrackedConstraint::DetachFromBodies()
{
    firstBody->NotifyConstraintRemoved(*this);

    if (optionalSecondBody != nullptr)
    {
        optionalSecondBody->NotifyConstraintRemoved(*this);
    }

    attachedToBodies = false;
}

} // namespace Thrive::Physics
