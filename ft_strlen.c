int	ft_strlen(char	*str)
{	
	void *temp = &str[0];

	if (!str)
		return (0);
	while (*str++ != '\0');
	return ((void*)str - temp - 1);
}