#include "BaseEntity.h"

std::vector<BaseEntity*> BaseEntity::Renderables;

BaseEntity::BaseEntity()
{
	// with no further information, we make some assumptions and set default values.
	filename = "Assets\\boid.png";
	colourTint = sf::Color::White;
}

BaseEntity::BaseEntity(std::string file) : filename(file)
{
	// with just a texture file, we default to a white tint (so no tint at all).
	colourTint = sf::Color::White;
}

BaseEntity::BaseEntity(std::string file, sf::Color colour) : filename(file), colourTint(colour)
{
	
}

BaseEntity::~BaseEntity()
{
	
}

void BaseEntity::Think()
{
	// set acceleration, somehow
}

void BaseEntity::Update()
{
	velocity += acceleration;

	// cap the velocity to a maximum value
	if (Magnitude(velocity) != 0.0f)
	{
		velocity /= Magnitude(velocity);
		velocity *= MaxBoidSpeed;
	}

	velocity *= 0.1f;

	// build a new position vector by adding a scaled version of the velocity vector
	sf::Vector2f pos = getPosition() + velocity;
	// update rotation based on the velocity
	float angle = atan2(velocity.y, velocity.x);
	setRotation(angle * RadToDeg);

	// wrap!
	if (pos.x > ScreenWidth)
		pos.x = 0.001f;
	if (pos.x < 0)
		pos.x = ScreenWidth;

	if (pos.y > ScreenHeight)
		pos.y = 0.001f;
	if (pos.y < 0)
		pos.y = ScreenHeight;

	// update our position
	setPosition(pos);

	acceleration *= 0.0f;
	sprite.setRotation(getRotation() * -1);
}

void BaseEntity::RandomisePosition()
{
	// set a random starting position.
	float startX = UtilRandom::instance()->GetRange(0, ScreenWidth);
	float startY = UtilRandom::instance()->GetRange(0, ScreenHeight);

	setPosition(startX, startY);
}

void BaseEntity::ResetVelocity()
{
		// select a random angle
	float angle = UtilRandom::instance()->GetRange(0, 361);
	// set our rotation value
	setRotation(angle);
	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f((float)cos(angle * M_PI / 180), (float)sin(angle * M_PI / 180));
}

sf::Vector2f BaseEntity::getVelocity()
{
	return velocity;
}

void BaseEntity::applyForce(const sf::Vector2f& force)
{
	acceleration += force;
}

void BaseEntity::Stop()
{
	velocity = sf::Vector2f(0,0);
	acceleration = sf::Vector2f(0, 0);
}

void BaseEntity::Initialize(bool _static)
{
	RandomisePosition();

	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(colourTint);
	
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

	acceleration.x = 0;
	acceleration.y = 0;

	if (!_static)
		ResetVelocity();

	// add the entity to the list of renderables.
	Renderables.push_back(this);
}