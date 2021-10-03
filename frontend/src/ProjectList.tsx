import React, {useEffect, useState} from 'react';
import {Button, Col, Dropdown, DropdownButton, Form, Modal, Spinner, Table} from "react-bootstrap";
import {useHistory} from 'react-router-dom';
import {DbProject} from './DatabaseInterface';
import axios from 'axios';
import CopyButton from './ui/CopyButton';
import RemoveButton from './ui/RemoveButton';
import OpenButton from './ui/OpenButton';
import Header from "./ui/Header";
import NewButton from "./ui/NewButton";
import formatDate from "./formatDate";
import './App.css';

type ProjectDialogProps = {
    show: boolean;
    onHide: () => void;
}

const ProjectDialog = (props: ProjectDialogProps) => {

    const [data, setData] = useState<DbProject>({
        id: 0,
        name: "",
        title: "",
        createdAt: new Date(),
        modifiedAt: new Date()
    })

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();
        await axios.post(process.env.REACT_APP_BACKEND + '/projects', data);
        //! \todo handle response
        window.location.reload();
    };

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new project</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Row>
                        <Form.Group as={Col} md="4" controlId="validationCustom01">
                            <Form.Label>Name</Form.Label>
                            <Form.Control
                                required
                                type="text"
                                name="name"
                                value={data.name}
                                onChange={handleInputChange}
                            />
                            <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                        </Form.Group>
                        <Form.Group as={Col} md="4" controlId="validationCustom02">
                            <Form.Label>Title</Form.Label>
                            <Form.Control
                                required
                                type="text"
                                name="title"
                                value={data.title}
                                onChange={handleInputChange}
                            />
                            <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                        </Form.Group>
                    </Form.Row>
                </Modal.Body>
                <Modal.Footer>
                    <Button type="submit">Submit</Button>
                    <Button onClick={props.onHide}>Close</Button>
                </Modal.Footer>
            </Form>
        </Modal>
    );
}

type ProjectListItemProps = {
    id: number;
    name: string;
    title: string;
    created: Date;
    modified: Date;
    onDeleteItem: (id: number) => void;
}

const ProjectListItem = (props: ProjectListItemProps) => {
    const history = useHistory();

    const onOpen = () => {
        history.push("/project/" + props.id);
    }
    const onCopy = () => {
        //! \todo
        console.log("TODO: copy project: ", props.id);
    }
    const onDelete = () => {
        props.onDeleteItem(props.id);
    }
    const onExportSAF = async () => {
        const response = await axios.get(process.env.REACT_APP_SERVICE_PROVIDER + '/export_to_saf', {params: {project: props.id}});
        const filename = response.data[0].data;
        window.open(process.env.REACT_APP_DOWNLOAD_URL + filename, "_blank");
    }
    return (
        <tr>
            <td><OpenButton onClick={onOpen} disabled={false}/></td>
            <td>{props.name}</td>
            <td>{props.title}</td>
            <td style={{color: "grey"}}>{formatDate(props.modified)}</td>
            <td style={{color: "grey"}}>{formatDate(props.created)}</td>
            <td><CopyButton onClick={onCopy} disabled={true}/></td>
            <td><RemoveButton onClick={onDelete} disabled={true}/></td>
            <td>
                <DropdownButton id="dropdown-basic-button" title="...">
                    <Dropdown.Item onClick={onExportSAF}>Export to SAF</Dropdown.Item>
                </DropdownButton>
            </td>
        </tr>
    )
}

const ProjectList = () => {

    const [data, setData] = useState<DbProject[]>([])
    const [loading, setLoading] = useState(true)
    const [showNewDialog, setShowNewDialog] = useState(false);

    const onDelete = (id: number) => {
        //! \todo
        console.log("TODO: delete project: ", id);
    }

    const getData = async () => {
        await axios.get(process.env.REACT_APP_BACKEND + '/projects')
            .then(response => setData(response.data))
            .finally(() => setLoading(false))
    }

    useEffect(() => {
        getData()
    }, []);

    if (loading)
        return (<Spinner animation="border" role="status"></Spinner>);

    return (
        <>
            <Header onBack={() => {}} disabledBackButton={true} title="List of Projects"/>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={false}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th></th>
                    <th>Name</th>
                    <th>Description</th>
                    <th>Last modified</th>
                    <th>Created</th>
                    <th></th>
                    <th></th>
                </tr>
                </thead>
                <tbody>
                {data.map(e => (
                    <ProjectListItem key={e.id} id={e.id} name={e.name} title={e.title} created={e.createdAt}
                                     modified={e.modifiedAt} onDeleteItem={onDelete}/>))}
                </tbody>
            </Table>

            <ProjectDialog
                show={showNewDialog}
                onHide={() => setShowNewDialog(false)}
            />
        </>
    );
}

export default ProjectList;
