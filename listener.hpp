/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygeslin <ygeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 17:16:47 by ygeslin           #+#    #+#             */
/*   Updated: 2022/01/31 15:01:40 by ygeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_HPP
#define LISTENER_HPP

class webserv;
#include "utils.hpp"

void	listener(webserv *server);
int		gen_listen_socket(int port);

#endif /* LISTENER_HPP */
